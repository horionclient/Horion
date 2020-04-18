#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include <vector>

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

class Logger {

public:
	static std::wstring GetRoamingFolderPath();
	static void WriteLogFileF(volatile char* fmt, ...);
	static void WriteBigLogFileF(size_t maxSize, const char* fmt, ...);
	static std::vector<TextForPrint>* GetTextToPrint();
	static CRITICAL_SECTION* GetTextToPrintSection();
	//static std::vector<TextForPrint*> stringPrintVector;
	static void Disable();
};

extern char logPath[200];
extern bool initializedLogger;
extern CRITICAL_SECTION loggerLock;
extern CRITICAL_SECTION vecLock;
extern std::vector<TextForPrint> stringPrintVector;
