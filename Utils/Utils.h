#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <random>
#include <vector>

//#include "xorstr.h"

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
	"INSERT",
	"DELETE",
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
	"OEM_3"};

#define INRANGE(x, a, b) (x >= a && x <= b)
#define GET_BYTE(x) (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xa) : (INRANGE(x, '0', '9') ? x - '0' : 0))

static inline float ImFmod(float x, float y) { return fmodf(x, y); }
static inline float ImFabs(float x) { return fabsf(x); }
template <typename T>
static inline void ImSwap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

#ifdef JM_XORSTR_HPP
#define FindSignature(szSignature) Utils::FindSignatureModule("Minecraft.Windows.exe", xorstr_(szSignature))
#else
#define FindSignature(szSignature) Utils::FindSignatureModule("Minecraft.Windows.exe", szSignature)
#endif

class Utils {
public:
	static inline unsigned int getCrcHash(const char* str, int seed = 0) {
		static unsigned int crc32_lut[256] = {0};
		if (!crc32_lut[1]) {
			const unsigned int polynomial = 0xEDB88320;
			for (unsigned int i = 0; i < 256; i++) {
				unsigned int crc = i;
				for (unsigned int j = 0; j < 8; j++)
					crc = (crc >> 1) ^ (((unsigned int)(-((int)(crc & 1)))) & polynomial);
				crc32_lut[i] = crc;
			}
		}

		seed = ~seed;
		unsigned int crc = seed;
		const unsigned char* current = (const unsigned char*)str;
		{
			// Zero-terminated string
			while (unsigned char c = *current++) {
				// We support a syntax of "label###id" where only "###id" is included in the hash, and only "label" gets displayed.
				// Because this syntax is rarely used we are optimizing for the common case.
				// - If we reach ### in the string we discard the hash so far and reset to the seed.
				// - We don't do 'current += 2; continue;' after handling ### to keep the code smaller.
				if (c == '#' && current[0] == '#' && current[1] == '#')
					crc = seed;
				crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
			}
		}
		return ~crc;
	}

	static inline const char* getKeybindName(int keybind) {
		return KeyNames[keybind];
	};
	// Convert rgb floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
	// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
	static inline void ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v) {
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
	static inline void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b) {
		if (s == 0.0f) {
			// gray
			out_r = out_g = out_b = v;
			return;
		}

		h = ImFmod(h, 1.0f) / (60.0f / 360.0f);
		int i = (int)h;
		float f = h - (float)i;
		float p = v * (1.0f - s);
		float q = v * (1.0f - s * f);
		float t = v * (1.0f - s * (1.0f - f));

		switch (i) {
		case 0:
			out_r = v;
			out_g = t;
			out_b = p;
			break;
		case 1:
			out_r = q;
			out_g = v;
			out_b = p;
			break;
		case 2:
			out_r = p;
			out_g = v;
			out_b = t;
			break;
		case 3:
			out_r = p;
			out_g = q;
			out_b = v;
			break;
		case 4:
			out_r = t;
			out_g = p;
			out_b = v;
			break;
		case 5:
		default:
			out_r = v;
			out_g = p;
			out_b = q;
			break;
		}
	};

	template <unsigned int IIdx, typename TRet, typename... TArgs>
	static inline auto CallVFunc(void* thisptr, TArgs... argList) -> TRet {
		//if (thisptr == nullptr)
		//return nullptr;
		using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
		return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
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

	static inline std::string randomString(const int size) {
		std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
		std::random_device rd;
		std::mt19937 generator(rd());
		std::shuffle(str.begin(), str.end(), generator);
		return str.substr(0, size);
	}

	static std::string getClipboardText();

	static void setClipboardText(std::string& text);

	static std::string readFileContents(std::wstring filePath) {
		std::ifstream fileStr(filePath, std::ios::in | std::ios::binary);
		if (fileStr) {
			std::string contents;
			fileStr.seekg(0, std::ios::end);
			contents.resize(fileStr.tellg());
			fileStr.seekg(0, std::ios::beg);
			fileStr.read(&contents[0], contents.size());
			fileStr.close();
			return contents;
		}
		return "";
	}

	static std::wstring wreadFileContents(std::wstring filePath) {
		std::wifstream fileStr(filePath, std::ios::in | std::ios::binary);
		if (fileStr) {
			std::wstring contents;
			fileStr.seekg(0, std::ios::end);
			contents.resize(fileStr.tellg());
			fileStr.seekg(0, std::ios::beg);
			fileStr.read(&contents[0], contents.size());
			fileStr.close();
			return contents;
		}
		return L"";
	}

	static uintptr_t FindSignatureModule(const char* szModule, const char* szSignature);

	static void GetCurrentSystemTime(tm& timeInfo);

	static void ApplySystemTime(std::stringstream* ss);

	static std::string sanitize(std::string text);

	static std::wstring stringToWstring(std::string txt);

	static bool endsWith(std::wstring const& fullString, std::wstring const& ending) {
		if (fullString.length() >= ending.length()) {
			return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
		} else {
			return false;
		}
	}

	static void ApplyRainbow(float* rcolors, const float modifier = 0.003f) {
		if (rcolors[3] < 1) {
			rcolors[0] = 1;
			rcolors[1] = 0.6f;
			rcolors[2] = 0.6f;
			rcolors[3] = 1;
		}

		Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);

		rcolors[0] += modifier;
		if (rcolors[0] >= 1)
			rcolors[0] = 0;

		Utils::ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);
	}

	static std::string getRttiBaseClassName(void* ptr);
};
