#pragma once

#include "MinHook.h"
#include "../Utils/Logger.h"
#include "../SDK/CGameMode.h"

class VMTHook;
class FuncHook;

class Hooks {
private:
public:
	static void Init();
	static void Restore();

private:
	static void __fastcall GameMode_tick(C_GameMode* _this);

	std::unique_ptr<FuncHook> gameMode_tickHook;

	typedef void(__fastcall* GameMode_tick_t)(C_GameMode* _this);
};

extern Hooks g_Hooks;

class FuncHook
{
public:
	void *funcPtr;
	void *funcReal;

	FuncHook(void* func, void* hooked) {
		funcPtr = func;
		int ret = MH_CreateHook(func, hooked, &funcReal);
		if (ret == MH_OK) {
			ret = MH_EnableHook(func);
			if(ret != MH_OK)
				logF("MH_EnableHook = %i", ret);
		}else
			logF("MH_CreateHook = %i", ret);
	};

	~FuncHook() {
		Restore();
	}

	void Restore() {
		MH_DisableHook(funcPtr);
	}

	template<class Type>
	Type GetOriginal()
	{
		return reinterpret_cast<Type>(funcReal);
	};
};

class VMTHook
{
public:
	VMTHook(void* ppClass)
	{
		this->ppBaseClass = static_cast<std::uintptr_t**>(ppClass);

		// loop through all valid class indexes. When it will hit invalid (not existing) it will end the loop
		while (static_cast<std::uintptr_t*>(*this->ppBaseClass)[this->indexCount])
			++this->indexCount;

		const std::size_t kSizeTable = this->indexCount * sizeof(std::uintptr_t);

		this->pOriginalVMT = *this->ppBaseClass;
		this->pNewVMT = std::make_unique<std::uintptr_t[]>(this->indexCount);

		// copy original vtable to our local copy of it
		std::memcpy(this->pNewVMT.get(), this->pOriginalVMT, kSizeTable);

		// replace original class with our local copy
		*this->ppBaseClass = this->pNewVMT.get();
	};
	~VMTHook() { *this->ppBaseClass = this->pOriginalVMT; };

	template<class Type>
	Type GetOriginal(const std::size_t index)
	{
		return reinterpret_cast<Type>(this->pOriginalVMT[index]);
	};

	HRESULT Hook(const std::size_t index, void* fnNew)
	{
		if (index > this->indexCount)   // check if given index is valid
			return E_INVALIDARG;

		this->pNewVMT[index] = reinterpret_cast<std::uintptr_t>(fnNew);
		return S_OK;
	};

	HRESULT Unhook(const std::size_t index)
	{
		if (index > this->indexCount)
			return E_INVALIDARG;

		this->pNewVMT[index] = this->pOriginalVMT[index];
		return S_OK;
	};

private:
	std::unique_ptr<std::uintptr_t[]> pNewVMT = nullptr;    // Actual used vtable
	std::uintptr_t** ppBaseClass = nullptr; // Saved pointer to original class
	std::uintptr_t*  pOriginalVMT = nullptr; // Saved original pointer to the VMT
	std::size_t      indexCount = 0;       // Count of indexes inside out f-ction
};