#pragma once

#ifndef PERFORMANCE_TEST
//#define PERFORMANCE_TEST
#endif

#include "MinHook.h"
#include "../SDK/CGameMode.h"
#include "../SDK/CChestBlockActor.h"
#include "../SDK/CBlockLegacy.h"
#include "GameData.h"
#include "../Horion/Config/ConfigManager.h"
#include "../SDK/TextHolder.h"
#include "../SDK/CMinecraftUIRenderContext.h"
#include "../Horion/Module/ModuleManager.h"
#include "../Horion/DrawUtils.h"
#include "../Horion/Command/CommandMgr.h"
#include "../Utils/TextFormat.h"
#include "../Horion/Menu/TabGui.h"
#include "../Horion/Menu/ClickGui.h"
#include "../SDK/CUIScene.h"
#include "../SDK/CMoveInputHandler.h"
#include "../SDK/CRakNetInstance.h"

#ifdef PERFORMANCE_TEST
#include <chrono>
#endif

#include <dxgi.h>

class VMTHook;
class FuncHook;

class Hooks {
private:
	bool shouldRender = true;
public:
	
	static void Init();
	static void Restore();

private:
	static void __fastcall GameMode_tick(C_GameMode* _this);
	static void __fastcall SurvivalMode_tick(C_GameMode* _this);
	static void __fastcall ChatScreenController_sendChatMessage(uint8_t* _this);
	static __int64 __fastcall UIScene_setupAndRender(C_UIScene* uiscene, __int64 screencontext);
	static __int64 __fastcall UIScene_render(C_UIScene* uiscene, __int64 screencontext);
	static __int64 __fastcall RenderText(__int64 a1, C_MinecraftUIRenderContext* renderCtx);
	static float* Dimension_getFogColor(__int64, float* color, float brightness);
	static void ChestBlockActor_tick(C_ChestBlockActor*, void* a);
	static void Actor_lerpMotion(C_Entity* _this, vec3_t);
	static signed int AppPlatform_getGameEdition(__int64 _this);
	static void PleaseAutoComplete(__int64 _this, __int64 a2, TextHolder* text, int a4);
	static void LoopbackPacketSender_sendToServer(C_LoopbackPacketSender* a, C_Packet* packet);
	static float LevelRendererPlayer_getFov(__int64 _this, float a2, bool a3);
	static void MultiLevelPlayer_tick(C_EntityList* entityList);
	static void GameMode_startDestroyBlock(C_GameMode* _this, vec3_ti* a2, uint8_t face, void* a4, void* a5);
	static void HIDController_keyMouse(C_HIDController* _this, void* a2, void* a3);
	static int __fastcall BlockLegacy_getRenderLayer(C_BlockLegacy* a1);
	static BYTE* __fastcall BlockLegacy_getLightEmission(C_BlockLegacy* _this, BYTE* a2);
	static __int64 LevelRenderer_renderLevel(__int64 _this, __int64 a2, __int64 a3);
	static void __fastcall ClickFunc(__int64 a1, char a2, char a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7, char a8);
	static __int64 __fastcall MoveInputHandler_tick(C_MoveInputHandler* _this, C_Entity* a2);
	static __int64 __fastcall ChestScreenController_tick(C_ChestScreenController* _this);
	static __int64 __fastcall GetGamma(__int64 a1);
	static bool __fastcall Actor_isInWater(C_Entity* _this);
	static void __fastcall JumpPower(C_Entity* _this, float a2);
	static __int64 __fastcall MinecraftGame_onAppSuspended(__int64 _this);
	static void __fastcall LadderUp(C_Entity* _this);
	static void __fastcall Actor_startSwimming(C_Entity* _this);
	static void __fastcall RakNetInstance_tick(C_RakNetInstance* _this);
	static float __fastcall GameMode_getPickRange(C_GameMode* _this, __int64 a2, char a3);
	static void __fastcall InventoryTransactionManager_addAction(C_InventoryTransactionManager* a1, C_InventoryAction* a2);

	std::unique_ptr<FuncHook> GameMode_tickHook;
	std::unique_ptr<FuncHook> SurvivalMode_tickHook;
	std::unique_ptr<FuncHook> ChatScreenController_sendChatMessageHook;
	std::unique_ptr<FuncHook> UIScene_setupAndRenderHook;
	std::unique_ptr<FuncHook> UIScene_renderHook;
	std::unique_ptr<FuncHook> RenderTextHook;
	std::unique_ptr<FuncHook> Dimension_getFogColorHook;
	std::unique_ptr<FuncHook> ChestBlockActor_tickHook;
	std::unique_ptr<FuncHook> Actor_lerpMotionHook;
	std::unique_ptr<FuncHook> AppPlatform_getGameEditionHook;
	std::unique_ptr<FuncHook> PleaseAutoCompleteHook;
	std::unique_ptr<FuncHook> LoopbackPacketSender_sendToServerHook;
	std::unique_ptr<FuncHook> LevelRendererPlayer_getFovHook;
	std::unique_ptr<FuncHook> MultiLevelPlayer_tickHook;
	std::unique_ptr<FuncHook> GameMode_startDestroyBlockHook;
	std::unique_ptr<FuncHook> HIDController_keyMouseHook;
	std::unique_ptr<FuncHook> BlockLegacy_getRenderLayerHook;
	std::unique_ptr<FuncHook> BlockLegacy_getLightEmissionHook;
	std::unique_ptr<FuncHook> LevelRenderer_renderLevelHook;
	std::unique_ptr<FuncHook> ClickFuncHook;
	std::unique_ptr<FuncHook> MoveInputHandler_tickHook;
	std::unique_ptr<FuncHook> ChestScreenController_tickHook;
	std::unique_ptr<FuncHook> GetGammaHook;
	std::unique_ptr<FuncHook> Actor_isInWaterHook;
	std::unique_ptr<FuncHook> JumpPowerHook;
	std::unique_ptr<FuncHook> MinecraftGame_onAppSuspendedHook;
	std::unique_ptr<FuncHook> LadderUpHook;
	std::unique_ptr<FuncHook> Actor_startSwimmingHook;
	std::unique_ptr<FuncHook> RakNetInstance_tickHook;
	std::unique_ptr<FuncHook> GameMode_getPickRangeHook;
	std::unique_ptr<FuncHook> InventoryTransactionManager_addActionHook;
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
		if (ret == MH_OK && (__int64)func > 10) {
			
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

	template<typename TRet, typename ... TArgs>
	auto* GetFastcall()
	{
		using Fn = TRet(__fastcall*)(TArgs...);
		return reinterpret_cast<Fn>(funcReal);
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