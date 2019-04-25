#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#ifdef _DEBUG
#include <ios>
#include <iostream>
#include <streambuf>
#include <iosfwd>
#endif

#include <windows.storage.h>
#include <wrl.h>
#include <mutex>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "Utils.h"

#pragma comment(lib,"runtimeobject")


#ifndef synchronized
#define synchronized(m) for(std::unique_lock<std::recursive_mutex> lk(m); lk; lk.unlock())
#endif

#ifndef logF 
#define logF WriteLogFileF
#endif

using namespace ABI::Windows::Storage;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

static std::recursive_mutex loggerMutex;

std::wstring GetRoamingFolderPath()
{
	ComPtr<IApplicationDataStatics> appDataStatics;
	auto hr = RoGetActivationFactory(HStringReference(L"Windows.Storage.ApplicationData").Get(), __uuidof(appDataStatics), &appDataStatics);
	if (FAILED(hr)) throw std::runtime_error("Failed to retrieve application data statics");

	ComPtr<IApplicationData> appData;
	hr = appDataStatics->get_Current(&appData);
	if (FAILED(hr)) throw std::runtime_error("Failed to retrieve current application data");

	ComPtr<IStorageFolder> roamingFolder;
	hr = appData->get_RoamingFolder(&roamingFolder);
	if (FAILED(hr)) throw std::runtime_error("Failed to retrieve roaming folder");

	ComPtr<IStorageItem> folderItem;
	hr = roamingFolder.As(&folderItem);
	if (FAILED(hr)) throw std::runtime_error("Failed to cast roaming folder to IStorageItem");

	HString roamingPathHString;
	hr = folderItem->get_Path(roamingPathHString.GetAddressOf());
	if (FAILED(hr)) throw std::runtime_error("Failed to retrieve roaming folder path");

	uint32_t pathLength;
	auto roamingPathCStr = roamingPathHString.GetRawBuffer(&pathLength);
	return std::wstring(roamingPathCStr, pathLength);
}

static void WriteLogFileF(const char* fmt, ...)
{
#ifdef _DEBUG
	try {
		synchronized(loggerMutex) {
			FILE* pFile;

			static char logPath[200];
			static bool yeet = false;
			if (!yeet) {
				std::wstring roam = GetRoamingFolderPath();
				sprintf_s(logPath, 200, "%S\\boi.txt", roam.c_str());
				yeet = true;
				try {
					remove(logPath);
				}
				catch (std::exception e) {
				}
			}

			fopen_s(&pFile, logPath, "a");

			std::stringstream ssTime;
			Utils::ApplySystemTime(&ssTime);


			fprintf(pFile, ssTime.str().c_str());
			va_list arg;

			va_start(arg, fmt);
			vfprintf(pFile, fmt, arg);
			va_end(arg);
			fprintf(pFile, "\n");

			fclose(pFile);
		}
	}
	catch (std::exception e) {
		// This throws an error when we cant acquire a lock on the logfile
		// That can happen when another process opens the file, or my code is shit and the mutex isnt doing what its supposed to
	}

#endif

}

static void WriteLogFile(const char* szString)
{
	WriteLogFileF("%s", szString);
}
