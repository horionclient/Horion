#pragma once

static const char* const KeyNames[] = {
	"Unknown",
	"VK_LBUTTON",
	"VK_RBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"Unknown",
	"VK_BACK",
	"TAB",
	"Unknown",
	"Unknown",
	"VK_CLEAR",
	"VK_RETURN",
	"Unknown",
	"Unknown",
	"Shift",
	"CTRL",
	"VK_MENU",
	"VK_PAUSE",
	"VK_CAPITAL",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"VK_ESCAPE",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"VK_SPACE",
	"VK_PRIOR",
	"VK_NEXT",
	"VK_END",
	"VK_HOME",
	"VK_LEFT",
	"VK_UP",
	"VK_RIGHT",
	"VK_DOWN",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"VK_SNAPSHOT",
	"VK_INSERT",
	"VK_DELETE",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"VK_LWIN",
	"VK_RWIN",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"NUMPAD0",
	"NUMPAD1",
	"NUMPAD2",
	"NUMPAD3",
	"NUMPAD4",
	"NUMPAD5",
	"NUMPAD6",
	"NUMPAD7",
	"NUMPAD8",
	"NUMPAD9",
	"VK_MULTIPLY",
	"VK_ADD",
	"VK_SEPARATOR",
	"VK_SUBTRACT",
	"VK_DECIMAL",
	"VK_DIVIDE",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"VK_F13",
	"VK_F14",
	"VK_F15",
	"VK_F16",
	"VK_F17",
	"VK_F18",
	"VK_F19",
	"VK_F20",
	"VK_F21",
	"VK_F22",
	"VK_F23",
	"VK_F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_NUMLOCK",
	"VK_SCROLL",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"LeftShift",
	"RightShift",
	"VK_LCONTROL",
	"VK_RCONTROL",
	"VK_LMENU",
	"VK_RMENU",
	"Browser Back",
	"Browser Forward",
	"Browser Refresh",
	"Browser Stop",
	"Browser Search",
	"Browser Favourites",
	"Browser Home",
	"Volume Mute",
	"Volume Down",
	"Volume Up",
	"Media_Next",
	"Media_Prev",
	"Media_Stop",
	"Media_Pause",
	"Mail",
	"Media",
	"App1",
	"App2",
	"Unknown",
	"Unknown",
	"OEM_1",
	"PLUS",
	"COMMA",
	"MINUS",
	"DOT",
	"OEM_2",
	"OEM_3"
};

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <ctime>
#include <Psapi.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <vector>
#include <random>
#include "Logger.h"

#define INRANGE(x,a,b)   (x >= a && x <= b)
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))

static inline float  ImFmod(float x, float y) { return fmodf(x, y); }
static inline float  ImFabs(float x) { return fabsf(x); }
template<typename T> static inline void ImSwap(T& a, T& b) { T tmp = a; a = b; b = tmp; }

class Utils {
public:
	static const char* getKeybindName(int keybind) {
		return KeyNames[keybind];
	};
	// Convert rgb floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
	// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
	static void ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v) {
		float K = 0.f;
		if (g < b) {
			ImSwap(g, b);
			K = -1.f;
		}
		if (r < g) {
			ImSwap(r, g);
			K = -2.f / 6.f - K;
		}

		const float chroma = r - (g < b ? g : b);
		out_h = ImFabs(K + (g - b) / (6.f * chroma + 1e-20f));
		out_s = chroma / (r + 1e-20f);
		out_v = r;
	};

	// Convert hsv floats ([0-1],[0-1],[0-1]) to rgb floats ([0-1],[0-1],[0-1]), from Foley & van Dam p593
	// also http://en.wikipedia.org/wiki/HSL_and_HSV
	static void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b) {
		if (s == 0.0f) {
			// gray
			out_r = out_g = out_b = v;
			return;
		}

		h = ImFmod(h, 1.0f) / (60.0f / 360.0f);
		int   i = (int)h;
		float f = h - (float)i;
		float p = v * (1.0f - s);
		float q = v * (1.0f - s * f);
		float t = v * (1.0f - s * (1.0f - f));

		switch (i) {
		case 0: out_r = v; out_g = t; out_b = p; break;
		case 1: out_r = q; out_g = v; out_b = p; break;
		case 2: out_r = p; out_g = v; out_b = t; break;
		case 3: out_r = p; out_g = q; out_b = v; break;
		case 4: out_r = t; out_g = p; out_b = v; break;
		case 5: default: out_r = v; out_g = p; out_b = q; break;
		}
	};

	template<unsigned int IIdx, typename TRet, typename ... TArgs>
	static auto CallVFunc(void* thisptr, TArgs ... argList) -> TRet {
		//if (thisptr == nullptr)
			//return nullptr;
		using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
		return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
	}

	static uintptr_t FindSignature(const char* szSignature) {
		return FindSignatureModule("Minecraft.Windows.exe", szSignature);
	}

	// https://stackoverflow.com/a/34571089
	static std::string base64_encode(const std::string& in) {

		std::string out;

		int val = 0, valb = -6;
		for (unsigned char c : in) {
			val = (val << 8) + c;
			valb += 8;
			while (valb >= 0) {
				out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
				valb -= 6;
			}
		}
		if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
		while (out.size() % 4) out.push_back('=');
		return out;
	}

	static std::string base64_decode(const std::string& in) {

		std::string out;

		std::vector<int> T(256, -1);
		for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

		int val = 0, valb = -8;
		for (unsigned char c : in) {
			if (T[c] == -1) break;
			val = (val << 6) + T[c];
			valb += 6;
			if (valb >= 0) {
				out.push_back(char((val >> valb) & 0xFF));
				valb -= 8;
			}
		}
		return out;
	}

	static std::string randomString(const int size) {
		std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
		std::random_device rd;
		std::mt19937 generator(rd());
		std::shuffle(str.begin(), str.end(), generator);
		return str.substr(0, size);
	}

	static uintptr_t FindSignatureModule(const char* szModule, const char* szSignature) {
		const char* pattern = szSignature;
		uintptr_t firstMatch = 0;
		static const uintptr_t rangeStart = (uintptr_t)GetModuleHandleA(szModule);
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
				if(*(PBYTE)pattern != '\?')
					patByte = GET_BYTE(pattern);
			}

			if (*(PBYTE)pattern == '\?' || *(BYTE*)pCur == patByte)
			{
				if (!firstMatch)
					firstMatch = pCur;

				if (!pattern[2])
					return firstMatch;

				//if (*(PWORD)pattern == '\?\?' || *(PBYTE)pattern != '\?')
				//pattern += 3;

				//else
					pattern += 2;
			}
			else {
				pattern = szSignature;
				firstMatch = 0;
			}
		}
#ifdef _DEBUG
		const char* sig = szSignature; // Put sig in here to access it in debugger
		// This will not get optimized away because we are in debug
		// Leave this in here to quickly find bad signatures in case of updates
#ifdef SIG_DEBUG
#ifdef logF
		logF("Signature dead: %s", szSignature);
#endif
#else
		char* msgToTheOverwhelmedDebugger = "SIGNATURE NOT FOUND";
		__debugbreak();

		throw std::exception("Signature not found");
#endif
#endif
		return 0u;
	}

	/**
	*   GetCurrentSystemTime - Gets actual system time
	*   @timeInfo: Reference to your own tm variable, gets modified.
	*/
	static void GetCurrentSystemTime(tm& timeInfo) {
		const std::chrono::system_clock::time_point systemNow = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(systemNow);
		localtime_s(&timeInfo, &now_c); // using localtime_s as std::localtime is not thread-safe.
	};


	static void ApplySystemTime(std::stringstream *ss) {

		using namespace std::chrono;
#ifdef _DEBUG
		const std::chrono::system_clock::time_point systemNow = system_clock::now(); 
		auto ms = duration_cast<milliseconds>(systemNow.time_since_epoch()) % 10000;
		time_t now_c = system_clock::to_time_t(systemNow);
		tm timeInfo{ };
		localtime_s(&timeInfo, &now_c); // using localtime_s as std::localtime is not thread-safe.
		*ss << "[" << std::put_time(&timeInfo, "%T") << "." << std::setfill('0') << std::setw(4) << ms.count() << "] ";
#endif
	};
};