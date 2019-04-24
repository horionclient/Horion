#pragma once


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <ctime>
#include <Psapi.h>
#include <iostream>

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
		DWORD firstMatch = 0;
		DWORD rangeStart = (DWORD)GetModuleHandleA(szModule);
		MODULEINFO miModInfo;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;

		BYTE patByte = GET_BYTE(pattern);
		const char* oldPat = pattern;

		for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
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
};