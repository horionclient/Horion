#include "Utils.h"

#include <iomanip>
#include <chrono>
#include <string>

#include "Logger.h"
#include <Windows.h>
#include <Psapi.h>

void Utils::ApplySystemTime(std::stringstream* ss) {
	using namespace std::chrono;
#ifdef _DEBUG
	const std::chrono::system_clock::time_point systemNow = system_clock::now();
	auto ms = duration_cast<milliseconds>(systemNow.time_since_epoch()) % 10000;
	time_t now_c = system_clock::to_time_t(systemNow);
	tm timeInfo{};
	localtime_s(&timeInfo, &now_c);  // using localtime_s as std::localtime is not thread-safe.
	*ss << "[" << std::put_time(&timeInfo, "%T") << "." << std::setfill('0') << std::setw(4) << ms.count() << "] ";
#endif
}

void Utils::GetCurrentSystemTime(tm& timeInfo) {
	const std::chrono::system_clock::time_point systemNow = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(systemNow);
	localtime_s(&timeInfo, &now_c);  // using localtime_s as std::localtime is not thread-safe.
}

bool invalidChar(char c) {
	return !(c >= 0 && *reinterpret_cast<unsigned char*>(&c) < 128);
}

std::string Utils::sanitize(std::string text) {
	std::string out;
	bool wasValid = true;
	for (char c : text) {
		bool isValid = !invalidChar(c);
		if (wasValid) {
			if (!isValid) {
				wasValid = false;
			} else {
				out += c;
			}
		} else {
			wasValid = isValid;
		}
	}
	return out;
}

std::wstring Utils::stringToWstring(std::string txt) {
	int wchars_num = MultiByteToWideChar(CP_UTF8, 0, txt.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[wchars_num];
	MultiByteToWideChar(CP_UTF8, 0, txt.c_str(), -1, wstr, wchars_num);
	// do whatever with wstr
	std::wstring gamer(wstr);
	delete[] wstr;
	return gamer;
}

std::string Utils::getRttiBaseClassName(void* ptr) {
#define retIfBad(m, c) \
	if (IsBadReadPtr(reinterpret_cast<void*>(m), c)) DebugBreak();

	retIfBad(ptr, 8);

	__int64 vtable = *reinterpret_cast<__int64*>(ptr);
	retIfBad(vtable - sizeof(uintptr_t), 8);

	uintptr_t moduleBase;
	if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, reinterpret_cast<char*>(vtable), reinterpret_cast<HMODULE*>(&moduleBase)))
		return std::string("invalid handle");

	auto objLocator = *reinterpret_cast<__int64*>(vtable - sizeof(uintptr_t));
	retIfBad(objLocator + 0x10, 8);
	auto classHierachyDescriptorOffset = *reinterpret_cast<int*>(objLocator + 0x10);
	auto classHierachy = moduleBase + classHierachyDescriptorOffset;
	retIfBad(classHierachy + 0x8, sizeof(unsigned int));
	int numBaseClasses = *reinterpret_cast<int*>(classHierachy + 0x8);
	if (numBaseClasses < 0 || numBaseClasses > 25)
		numBaseClasses = 0;
	retIfBad(classHierachy + 0xC, sizeof(uintptr_t));
	auto baseClassArrayOffset = *reinterpret_cast<int*>(classHierachy + 0xC);
	auto baseClassArray = moduleBase + baseClassArrayOffset;
	retIfBad(baseClassArray, sizeof(unsigned int));


	// We could iterate over all base classes here, but we just return the first
	auto classDescriptorOffset = *reinterpret_cast<unsigned int*>(baseClassArray);
	auto classDescriptor = moduleBase + classDescriptorOffset;
	retIfBad(classDescriptor, sizeof(int));
	auto typeDescripterOffset = *reinterpret_cast<int*>(classDescriptor);
	auto typeDescriptor = moduleBase + typeDescripterOffset;
	retIfBad(typeDescriptor, 0x10 + 512);

	size_t rttiLength = strnlen_s(reinterpret_cast<char*>(typeDescriptor + 0x11), 512);
	if (rttiLength > 5 && rttiLength < 512) {
		auto name = std::string(reinterpret_cast<char*>(typeDescriptor + 0x10 + 1));
		if (name.substr(0, 3) == "?AV")
			name = name.substr(3);
		if (name.substr(name.size() - 2, 2) == "@@")
			name = name.substr(0, name.size() - 2);
		return name;
	}

	return std::string("invalid");
}
std::string Utils::getClipboardText() {
	if (!OpenClipboard(nullptr)) {
		return "";
	} else {
		HANDLE hData = GetClipboardData(CF_TEXT);
		char* pszText = static_cast<char*>(GlobalLock(hData));
		if (pszText == nullptr)
			return "";
		CloseClipboard();
		return std::string(pszText);
	}
}
void Utils::setClipboardText(std::string& text) {
	if (!OpenClipboard(nullptr))
		return;
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), text.c_str(), text.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}
uintptr_t Utils::FindSignatureModule(const char* szModule, const char* szSignature) {
	const char* pattern = szSignature;
	uintptr_t firstMatch = 0;
	static const auto rangeStart = (uintptr_t)GetModuleHandleA(szModule);
	static MODULEINFO miModInfo;
	static bool init = false;
	if (!init) {
		init = true;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	}
	static const uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;

	BYTE patByte = GET_BYTE(pattern);
	const char* oldPat = pattern;

	for (uintptr_t pCur = rangeStart; pCur < rangeEnd; pCur++) {
		if (!*pattern)
			return firstMatch;

		while (*(PBYTE)pattern == ' ')
			pattern++;

		if (!*pattern)
			return firstMatch;

		if (oldPat != pattern) {
			oldPat = pattern;
			if (*(PBYTE)pattern != '\?')
				patByte = GET_BYTE(pattern);
		}

		if (*(PBYTE)pattern == '\?' || *(BYTE*)pCur == patByte) {
			if (!firstMatch)
				firstMatch = pCur;

			if (!pattern[2] || !pattern[1])
				return firstMatch;

			//if (*(PWORD)pattern == '\?\?' || *(PBYTE)pattern != '\?')
			//pattern += 3;

			//else
			pattern += 2;
		} else {
			pattern = szSignature;
			firstMatch = 0;
		}
	}
#ifdef _DEBUG
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
	const char* sig = szSignature;  // Put sig in here to access it in debugger
	// This will not get optimized away because we are in debug
	// Leave this in here to quickly find bad signatures in case of updates
	logF("Signature dead: %s", szSignature);
	if (true) {
		const char* msgToTheOverwhelmedDebugger = "SIGNATURE NOT FOUND";
		__debugbreak();
	}

#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif
	return 0u;
}
