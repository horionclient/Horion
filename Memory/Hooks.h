#pragma once

#include "MinHook.h"
#include "../SDK/CGameMode.h"
#include "GameData.h"
#include "../Directx/Directx.h"
#include "../SDK/TextHolder.h"
#include "../SDK/CMinecraftUIRenderContext.h"
#include "../Horion/Module/ModuleManager.h"

#include <dxgi.h>

class VMTHook;
class FuncHook;
struct BigCantWork;

class Hooks {
private:
public:

	static void Init();
	static void Restore();

private:
	static void __fastcall GameMode_tick(C_GameMode* _this);
	static void __fastcall ChatScreenController_sendChatMessage(uint8_t* _this);
	static HRESULT __stdcall d3d11_present(IDXGISwapChain * pSwapChain, UINT SyncInterval, UINT Flags);
	static __int64 __fastcall renderText(__int64 yeet, C_MinecraftUIRenderContext* yote);
	static char* __fastcall I8n_get(void*, char*);
	static TextHolder* __fastcall Options_getVersionString(void*, __int64);
	static float* Dimension_getFogColor(__int64, float* color, float brightness);
	static void GameMode_destroyBlock(void*, C_BlockPos*, uint8_t face);

	std::unique_ptr<FuncHook> gameMode_tickHook;
	std::unique_ptr<FuncHook> chatScreen_sendMessageHook;
	std::unique_ptr<FuncHook> d3d11_presentHook;
	std::unique_ptr<FuncHook> renderTextHook;
	std::unique_ptr<FuncHook> I8n_getHook;
	std::unique_ptr<FuncHook> Options_getVersionStringHook;
	std::unique_ptr<FuncHook> Dimension_getFogColorHook;
	std::unique_ptr<FuncHook> GameMode_destroyBlockHook;

	typedef __int64(__fastcall* GameMode_destroyBlock_t)(void*, C_BlockPos*, uint8_t face);
	typedef float*(__fastcall* Dimension_getFogColor_t)(__int64, float*, __int64);
	typedef char*(__fastcall* I8n_get_t)(void*, char*);
	typedef __int64(__fastcall* renderText_t)(__int64, C_MinecraftUIRenderContext*);
	typedef void(__fastcall* GameMode_tick_t)(C_GameMode* _this);
	typedef void(__fastcall* ChatScreen_sendChatMessage_t)(void* _this);
	typedef TextHolder*(__fastcall* Options_getVersionString_t)(void* _this, __int64);
	typedef HRESULT(__stdcall* d3d11_present_t)(IDXGISwapChain * pSwapChain, UINT SyncInterval, UINT Flags);
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
			
		}else
			logF("MH_CreateHook = %i", ret);
	};

	void init() {
		int ret = MH_EnableHook(funcPtr);
		if (ret != MH_OK)
			logF("MH_EnableHook = %i", ret);
	}

	~FuncHook() {
		Restore();
	}

	void Restore() {
		if(this != nullptr && funcPtr != nullptr)
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