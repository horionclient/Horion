#pragma once


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

#define INRANGE(x,a,b)   (x >= a && x <= b)
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))

class Utils
{
public:
	template<unsigned int IIdx, typename TRet, typename ... TArgs>
	static auto CallVFunc(void* thisptr, TArgs ... argList) -> TRet
	{
		//if (thisptr == nullptr)
			//return nullptr;
		using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
		return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
	}

	static uintptr_t FindSignature(const char* szSignature) {
		return FindSignatureModule("Minecraft.Windows.exe", szSignature);
	}

	static uintptr_t FindSignatureModule(const char* szModule, const char* szSignature)
	{
		const char* pattern = szSignature;
		uintptr_t firstMatch = 0;
		uintptr_t rangeStart = (uintptr_t)GetModuleHandleA(szModule);
		MODULEINFO miModInfo;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;

		BYTE patByte = GET_BYTE(pattern);
		const char* oldPat = pattern;

		for (uintptr_t pCur = rangeStart; pCur < rangeEnd; pCur++)
		{
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
			else
			{
				pattern = szSignature;
				firstMatch = 0;
			}
		}
		return 0u;
	}

	/**
	*   GetCurrentSystemTime - Gets actual system time
	*   @timeInfo: Reference to your own tm variable, gets modified.
	*/
	static void GetCurrentSystemTime(tm& timeInfo)
	{
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