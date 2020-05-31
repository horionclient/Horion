#pragma once

/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** SlimMem.h
** A simple to use memory-manipulation class
**
** Author: Zat
** -------------------------------------------------------------------------*/

// clang-format off
//#include <windows.h>   //Windows-functions (OpenProcess, RPM, WPM, etc)
//#include <tlhelp32.h>  //Functions that gather process-information
//#include <Psapi.h>     //Functions that gather module-information

#include <algorithm>    //transform-function that is used to apply the tolower-function to a wstring
#include <cassert>      //Used for debugging
#include <cctype>       //tolower-function that converts a char to lowercase
//#include <iostream>     //cout
#include <map>          //Data-container that saves parsed modules
#include <memory>       //unique_ptr
#include <string>       //String/WString implementation
#include <type_traits>  //Used to restrict usage of template functions Read and Write to specific types
#include <vector>

// clang-format on

namespace SlimUtils {

//Exclude module-names from SlimModule-structs
#define VERYSLIM_SLIMMODULE
//Enable error-reports
#define REPORT_ERRORS

struct SlimModule;
struct SigScanResult;
class SlimMem;

/*
	Contains basic information about a single module
	*/
struct SlimModule {
	std::uintptr_t ptrBase;
	unsigned long dwSize;

	SlimModule(std::uintptr_t base, unsigned long baseSize) {
		ptrBase = base;
		dwSize = baseSize;
	}
};

/*
	Holds information about a signature-scan
	*/
struct SigScanResult {
	bool m_Success;
	unsigned char* m_Data = 0;
	unsigned long m_DataLength;
	unsigned long m_Offset;

	SigScanResult() : m_Success(false), m_Data(nullptr), m_DataLength(0), m_Offset(0) {
	}

	SigScanResult(bool p_Success) : m_Success(p_Success), m_Data(nullptr), m_DataLength(0), m_Offset(0) {
	}

	SigScanResult(bool p_Success, unsigned long p_Offset, unsigned char* p_Data, unsigned long p_DataLength) : m_Success(p_Success), m_DataLength(p_DataLength), m_Offset(p_Offset) {
		if (p_Data != nullptr) {
			m_Data = new unsigned char[m_DataLength];
			memcpy_s(m_Data, m_DataLength, p_Data, m_DataLength);
		}
	}

	SigScanResult(const SigScanResult& other) : SigScanResult(other.m_Success, other.m_Offset, other.m_Data, other.m_DataLength) {
	}

	~SigScanResult() {
		if (m_Data != nullptr)
			delete[] m_Data;
	}

	SigScanResult& operator=(const SigScanResult& other) {
		if (this->m_Data != nullptr) {
			delete[] this->m_Data;
			this->m_Data = nullptr;
		}
		this->m_Success = other.m_Success;
		this->m_Offset = other.m_Offset;

		if (other.m_Data != nullptr) {
			this->m_DataLength = other.m_DataLength;
			this->m_Data = new unsigned char[other.m_DataLength];
			memcpy_s(this->m_Data, this->m_DataLength, other.m_Data, other.m_DataLength);
		}
		return *this;
	}

	template <typename T>
	bool Read(T& value, unsigned long index) const {
		if (index + sizeof(T) >= m_DataLength)
			return false;

		value = *(T*)(reinterpret_cast<unsigned long>(m_Data) + index);
		return true;
	}
};

/*
	Offers a simple collection of combination of process-access flags
	*/
enum ProcessAccess : unsigned long {
	Full = ((0x000F0000L) | (0x00100000L) | 0xFFFF),
	ReadOnly = 0x0008 | 0x0010,
	WriteOnly = 0x0008 | 0x0020,
	ReadWrite = ReadOnly | WriteOnly
};

/*
	A class that provides basic functions that are used to write to/read from process memory
	*/
class SlimMem {
public:
	SlimMem() : m_hProc((void*)-1), m_dwPID(0) {}
	SlimMem(const SlimMem& copy);
	~SlimMem();

	SlimMem& operator=(const SlimMem& other) {
		this->m_hProc = other.m_hProc;
		this->m_dwPID = other.m_dwPID;
		return *this;
	}

	bool Open(const wchar_t* lpwstrProcessName, ProcessAccess flags);
	bool Open(const wchar_t* lpwstrProcessName, unsigned long dwFlags);
	bool Open(unsigned long dwPID, ProcessAccess flags);
	bool Open(unsigned long dwPID, unsigned long dwFlags);
	void Close();

	bool HasProcessHandle() const;
	const SlimModule* GetModule(const wchar_t* lpwstrModuleName) const;
	bool ParseModules();
	SigScanResult PerformSigScan(const unsigned char* bufPattern, const char* lpcstrMask, const SlimModule* Module, unsigned long startFromOffset);

	template <typename T>
	T Read(std::uintptr_t ptrAddress) const;

	//		template <typename T>
	//bool Read(std::uintptr_t ptrAddress, T& value) const;

	//unsigned char* ReadRaw(std::uintptr_t ptrAddress, int size) const;
	//wchar_t * ReadRawWide(std::uintptr_t ptrAddress, int size) const;

	template <typename T>
	T ReadPtr(std::uintptr_t ptrAddress, std::initializer_list<std::uintptr_t> ilOffsets) const;

	template <typename T>
	bool Write(std::uintptr_t ptrAddress, T value) const;

	//inline bool WriteRaw(std::uintptr_t ptrAddress, uint8_t value[], size_t length) const;
	//inline bool WriteRawChar(std::uintptr_t ptrAddress, char value[], size_t length) const;

	static bool GetPID(const wchar_t* lpwstrProcessName, unsigned long* pid);
	void* m_hProc;
	unsigned long m_dwPID;
	std::map<std::wstring, std::unique_ptr<SlimModule>> m_mModules;

private:
};

#pragma region Read / Write
/*
	Reads data from memory
	Returns the value read from memory; Returns default-value in case an error occured
	*/
template <typename T>
inline T SlimMem::Read(std::uintptr_t ptrAddress) const {
	//static_assert(std::is_trivially_copyable<T>::value, "Invalid RPM/WPM type");

	//T val = T();
	//if (!this->HasProcessHandle())
	//return val;

	return *reinterpret_cast<T*>(ptrAddress);
	//ReadProcessMemory(this->m_hProc, (LPCVOID)ptrAddress, &val, sizeof(T), NULL);
	//return val;
}

/*
	Reads data from memory
	Returns whether the operation was successful or not!
	*/
/*template<typename T>
	inline bool SlimUtils::SlimMem::Read(std::uintptr_t ptrAddress, T & value) const
	{
		static_assert(std::is_trivially_copyable<T>::value, "Invalid RPM/WPM type");

		SIZE_T unsigned charsRead;

		if (!this->HasProcessHandle())
			return false;

		return ReadProcessMemory(m_hProc, (LPCVOID)ptrAddress, &value, sizeof(T), &bytesRead) && bytesRead == sizeof(T);
	}



	inline unsigned char * SlimMem::ReadRaw(std::uintptr_t ptrAddress, int size) const
	{
		//static_assert(std::is_trivially_copyable<T>::value, "Invalid RPM/WPM type");

		unsigned char* arr = new unsigned char[size];
		if (!this->HasProcessHandle())
			return arr;


		ReadProcessMemory(this->m_hProc, (LPCVOID)ptrAddress, (LPVOID)arr, size, NULL);
		return arr;
	}

	inline wchar_t* SlimMem::ReadRawWide(std::uintptr_t ptrAddress, int size) const
	{
		//static_assert(std::is_trivially_copyable<T>::value, "Invalid RPM/WPM type");

		wchar_t* arr = new wchar_t[size];
		if (!this->HasProcessHandle())
			return arr;

		ReadProcessMemory(this->m_hProc, (LPCVOID)ptrAddress, (LPVOID)arr, size * sizeof(wchar_t), NULL);
		return arr;
	}*/

/*
	Reads data from memory
	Returns the value read from memory; Returns default-value in case an error occured
	*/
template <typename T>
inline T SlimMem::ReadPtr(std::uintptr_t ptrAddress, std::initializer_list<std::uintptr_t> ilOffsets) const {
	static_assert(std::is_trivially_copyable<T>::value, "Invalid RPM/WPM type");

	if (!this->HasProcessHandle())
		return 0x0;
	for (auto it = ilOffsets.begin(); it != ilOffsets.end(); it++) {
		if ((std::uintptr_t*)(it + 1) == ilOffsets.end()) {
			//Read value
			//std::cout << "yee: " << std::hex << ptrAddress << ":" << std::hex << *it << std::endl;
			return this->Read<T>(ptrAddress + *it);
		} else {
			//Read offset
			//std::cout << "yee: " << std::hex << ptrAddress << ":" << std::hex << *it << std::endl;
			ptrAddress = this->Read<std::uintptr_t>(ptrAddress + *it);
		}
	}
	return T();
}

/*
	Writes data to memory
	Returns whether the operation was successful or not!
	*/
template <typename T>
inline bool SlimMem::Write(std::uintptr_t ptrAddress, T value) const {
	//static_assert(std::is_trivially_copyable<T>::value, "Invalid RPM/WPM type");

	//if (!this->HasProcessHandle())
	//return false;
	*reinterpret_cast<T*>(ptrAddress) = value;
	return true;
	//return WriteProcessMemory(this->m_hProc, (LPVOID)ptrAddress, &value, sizeof(T), NULL);
}

/*inline bool SlimMem::WriteRaw(std::uintptr_t ptrAddress, uint8_t value[], size_t size) const
	{
		//static_assert(std::is_trivially_copyable<T>::value, "Invalid RPM/WPM type");

		if (!this->HasProcessHandle())
			return false;
		return WriteProcessMemory(this->m_hProc, (LPVOID)ptrAddress, value, size, NULL);
	}

	inline bool SlimMem::WriteRawChar(std::uintptr_t ptrAddress, char value[], size_t size) const
	{
		//static_assert(std::is_trivially_copyable<T>::value, "Invalid RPM/WPM type");

		if (!this->HasProcessHandle())
			return false;
		return WriteProcessMemory(this->m_hProc, (LPVOID)ptrAddress, value, size, NULL);
	}*/
#pragma endregion
}  // namespace SlimUtils
