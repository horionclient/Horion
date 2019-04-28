#include "Logger.h"

char logPath[200]; 
bool yeet = false;
CRITICAL_SECTION loggerLock;
CRITICAL_SECTION vecLock;
std::vector<TextForPrint*> stringPrintVector = std::vector<TextForPrint*>();

std::wstring Logger::GetRoamingFolderPath()
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

void Logger::WriteLogFileF(const char * fmt, ...)
{
#ifdef _DEBUG
	//try {

	FILE* pFile;


	if (!yeet) {
		yeet = true;
		InitializeCriticalSection(&loggerLock);
		EnterCriticalSection(&loggerLock);
		InitializeCriticalSection(&vecLock);

		std::wstring roam = GetRoamingFolderPath();
		sprintf_s(logPath, 200, "%S\\boi.txt", roam.c_str());

		try {
			remove(logPath);
		}
		catch (std::exception e) {
		}

	}
	else EnterCriticalSection(&loggerLock);

	pFile = _fsopen(logPath, "a", _SH_DENYWR);
	if (pFile != nullptr)
	{
		std::stringstream ssTime;
		Utils::ApplySystemTime(&ssTime);

		char bigboi[300];
		char smolBoi[20];
		sprintf_s(smolBoi, 20, "%s", ssTime.str().c_str());

		va_list arg;
		va_start(arg, fmt);
		int numCharacters = vsprintf_s(bigboi, 300, fmt, arg);
		va_end(arg);
		fprintf(pFile, "%s %s", smolBoi, bigboi);
		fprintf(pFile, "\n");

		fclose(pFile);

		if (numCharacters < 70) {
			TextForPrint boeing747;
			boeing747.length = sprintf_s(boeing747.text, 70, "%s %s", smolBoi, bigboi) + 1;
			EnterCriticalSection(&vecLock);
			stringPrintVector.push_back(&boeing747);
			LeaveCriticalSection(&vecLock);
		}
	}

	//}
	//catch (std::exception e) {
	//	throw e;
		// This throws an error when we cant acquire a lock on the logfile
		// That can happen when another process opens the file, or my code is shit and the mutex isnt doing what its supposed to
	//}
	LeaveCriticalSection(&loggerLock);
#endif
}

std::vector<TextForPrint*> Logger::GetTextToPrint()
{
	return stringPrintVector;
}

CRITICAL_SECTION* Logger::GetTextToPrintSection()
{
	return &vecLock;
}

void Logger::Disable()
{
#ifdef _DEBUG
	DeleteCriticalSection(&loggerLock);
	DeleteCriticalSection(&vecLock);
#endif

}
