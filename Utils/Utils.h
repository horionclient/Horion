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

static inline float  ImFmod(float x, float y) { return fmodf(x, y); }
static inline float  ImFabs(float x) { return fabsf(x); }
template<typename T> static inline void ImSwap(T& a, T& b) { T tmp = a; a = b; b = tmp; }

class Utils
{
public:


	// Convert rgb floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
	// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
	static void ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
	{
		float K = 0.f;
		if (g < b)
		{
			ImSwap(g, b);
			K = -1.f;
		}
		if (r < g)
		{
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
	static void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
	{
		if (s == 0.0f)
		{
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

		switch (i)
		{
		case 0: out_r = v; out_g = t; out_b = p; break;
		case 1: out_r = q; out_g = v; out_b = p; break;
		case 2: out_r = p; out_g = v; out_b = t; break;
		case 3: out_r = p; out_g = q; out_b = v; break;
		case 4: out_r = t; out_g = p; out_b = v; break;
		case 5: default: out_r = v; out_g = p; out_b = q; break;
		}
	};

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