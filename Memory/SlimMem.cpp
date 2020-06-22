#include "SlimMem.h"
#include <windows.h>
#include <tlhelp32.h>
#include <Psapi.h>

inline bool IsProcessHandleValid(HANDLE h) { return h > (void*)0 && h != INVALID_HANDLE_VALUE; };
inline bool IsHandleValid(HANDLE h) { return h != INVALID_HANDLE_VALUE; }
inline BOOL ProperlyCloseHandle(HANDLE h) {
	auto const b = CloseHandle(h);
	assert(b);
	return b;
}

static std::wstring ToLower(std::wstring string) {
	transform(string.begin(), string.end(), string.begin(), tolower);
	return string;
}

namespace SlimUtils {

bool SlimMem::HasProcessHandle() const { return IsProcessHandleValid(m_hProc); }

#pragma region Constructors/Destructors
	SlimMem::SlimMem(const SlimMem & copy)
	{
		this->m_dwPID = 0;
		DuplicateHandle(GetCurrentProcess(), copy.m_hProc, GetCurrentProcess(), &m_hProc, NULL, FALSE, DUPLICATE_SAME_ACCESS);
	}

	SlimMem::~SlimMem()
	{
		this->Close();
	}
#pragma endregion

#pragma region Open/Close
	void SlimMem::Close()
	{
		m_mModules.clear();

		//Close the handle to the process in case it's still open
		if (IsProcessHandleValid(m_hProc)) {
			ProperlyCloseHandle(m_hProc);
		}
	}

	bool SlimMem::Open(const wchar_t * lpwstrProcessName, ProcessAccess flags)
	{
		return this->Open(lpwstrProcessName, (DWORD)flags);
	}

	bool SlimMem::Open(const wchar_t * lpwstrProcessName, DWORD flags)
	{
		DWORD pid;
		if (GetPID(lpwstrProcessName, &pid))
			return this->Open(pid, flags);
		return false;
	}

	bool SlimMem::Open(DWORD dwPID, ProcessAccess flags)
	{
		return this->Open(dwPID, (DWORD)flags);
	}

	bool SlimMem::Open(DWORD dwPID, DWORD dwFlags)
	{
		if (this->HasProcessHandle()) {

			return false;
		}


		m_hProc = OpenProcess(dwFlags | PROCESS_DUP_HANDLE, false, dwPID);
		m_dwPID = dwPID;
		if (this->HasProcessHandle())
			this->ParseModules();


		return this->HasProcessHandle();
	}
#pragma endregion

#pragma region Utility
	/*
	Attempts to find a process with a given name and sets the given PID
	Returns whether a matching process was found or not
	*/
	bool SlimMem::GetPID(const wchar_t * lpwstrProcessName, DWORD* pid)
	{
		PROCESSENTRY32W proc;
		proc.dwSize = sizeof(PROCESSENTRY32W);
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		*pid = 0;

		if (!IsHandleValid(hSnap))
			return false;

		if (Process32FirstW(hSnap, &proc)) {
			do {
				if (wcscmp(lpwstrProcessName, proc.szExeFile) == 0)
				{
					ProperlyCloseHandle(hSnap);
					*pid = proc.th32ProcessID;
					return true;
				}
			} while (Process32NextW(hSnap, &proc));
		}

		ProperlyCloseHandle(hSnap);
		return false;
	}

	/*
	Caches basic information of modules loaded by the opened-process

	*/
	bool SlimMem::ParseModules()
	{
		if (!this->HasProcessHandle())
			return false;

		m_mModules.clear();

		MODULEENTRY32W mod;
		mod.dwSize = sizeof(MODULEENTRY32W);

		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwPID);
		if (!IsHandleValid(hSnap))
			return false;

		if (Module32FirstW(hSnap, &mod)) {
			do {
				try {
					if (m_mModules.find(std::wstring(mod.szModule)) == m_mModules.end())
						m_mModules[ToLower(mod.szModule)] = std::make_unique<SlimModule>((std::uintptr_t) mod.modBaseAddr, mod.modBaseSize);
				}
				catch (...) {

				}
			} while (Module32NextW(hSnap, &mod));
		}

		ProperlyCloseHandle(hSnap);
		return true;
	}

	SigScanResult SlimMem::PerformSigScan(const BYTE * bufPattern, const char * lpcstrMask, const SlimModule * Module, DWORD startFromOffset)
	{
		auto module = Module;
		if (module == nullptr)
			return SigScanResult(false);
		std::string mask(lpcstrMask);

		if (mask.empty())
			return SigScanResult(false);

		if (module->dwSize <= startFromOffset)
			return SigScanResult(false);

		if (startFromOffset > module->dwSize - mask.size())
			return SigScanResult(false);

		if (mask[0] != 'x') {

			return SigScanResult(false);
		}



		BYTE *dump = new BYTE[module->dwSize];

		SIZE_T bytesRead;

		if (!ReadProcessMemory(this->m_hProc, (LPCVOID)module->ptrBase, dump, module->dwSize, &bytesRead) || bytesRead != module->dwSize)
			return SigScanResult(false);

		bool found = false;
		size_t maskSize = mask.size();
		const char * goodMask = mask.c_str();
		DWORD count = (DWORD)(module->dwSize - maskSize);
		for (DWORD i = startFromOffset; i < count; i++) {
			if (bufPattern[0] == dump[i]) {
				found = true;
				for (DWORD idx = 1; idx < maskSize; idx++) {

					if (goodMask[idx] == 0x78 && bufPattern[idx] != dump[i + idx]) {
						found = false;
						break;
					}
				}
				if (found) {
					SigScanResult result(true, i, dump + i, (DWORD)maskSize);
					delete[] dump;
					return result;
				}
			}
		}
		delete[] dump;

		return SigScanResult(false);
	}

	const SlimModule* SlimMem::GetModule(const wchar_t * lpwstrModuleName) const
	{
		std::wstring name = ToLower(std::wstring(lpwstrModuleName));
		auto val = m_mModules.find(name);
		if (val == m_mModules.end())
			return nullptr;

		return (*val).second.get();
	}

#pragma endregion
}