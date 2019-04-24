#pragma once

#include <Windows.h>
#include <random>
#include <iostream>
#include <mutex>
#include "Utils.h"

#ifndef synchronized(m)
#define synchronized(m) for(std::unique_lock<std::recursive_mutex> lk(m); lk; lk.unlock())
#endif

#ifndef logF 
#define logF WriteLogFileF
#endif

static std::recursive_mutex loggerMutex;

static void WriteLogFileF(const char* fmt, ...)
{
#ifdef _DEBUG
	
		
	synchronized(loggerMutex) {
		FILE* pFile;
		fopen_s(&pFile, "C:\\Users\\gbaker\\AppData\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\boi.txt", "a");
		va_list arg;

		va_start(arg, fmt);
		vfprintf(pFile, fmt, arg);
		va_end(arg);
		fprintf(pFile, "\n");

		fclose(pFile);
	}


#endif

}

static void WriteLogFile(const char* szString)
{
	WriteLogFileF("%s", szString);
}