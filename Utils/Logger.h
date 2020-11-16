#pragma once

#include <mutex>
#include <vector>
#include <memory>

#ifdef _DEBUG
#include <ios>
#include <iosfwd>
#include <iostream>
#endif

#include "xorstr.h"
#include <string>

#pragma comment(lib, "runtimeobject")

#ifndef logF
//#define logF(x) Logger::WriteLogFileF(XorString(x))
#define logF(x, ...) Logger::WriteLogFileF(XorString(x), __VA_ARGS__)
#endif

struct TextForPrint {
	char time[20];
	char text[100];
};

struct TextForPrintBig {
	char time[20];
	char text[2900];
};

class Logger {

public:
	static bool isActive();
	static std::wstring GetRoamingFolderPath();
	static void WriteLogFileF(volatile char* fmt, ...);
	static void WriteBigLogFileF(size_t maxSize, const char* fmt, ...);
	static void SendToConsoleF(const char* msg);
	static std::vector<TextForPrint>* GetTextToPrint();
	static std::vector<std::shared_ptr<TextForPrintBig>>* GetTextToSend();
	static std::lock_guard<std::mutex> GetTextToPrintLock();
	static std::lock_guard<std::mutex> GetTextToInjectorLock();
	//static std::vector<TextForPrint*> stringPrintVector;
	static void Disable();
};
