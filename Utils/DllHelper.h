#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <type_traits>

class ProcPtr {
public:
	explicit ProcPtr(FARPROC ptr) : _ptr(ptr) {}

	template <typename T, typename = std::enable_if_t<std::is_function_v<T>>>
	operator T*() const {
		return reinterpret_cast<T*>(_ptr);
	}

private:
	FARPROC _ptr;
};

class DllHelper {
private:
	HMODULE _module;

public:
	static HMODULE _parent_module;

	explicit DllHelper(LPCSTR filename) : _module(GetModuleHandleA(filename)) {}

	~DllHelper() {}

	ProcPtr operator[](LPCSTR proc_name) const {
		return ProcPtr(GetProcAddress(_module, proc_name));
	}
};