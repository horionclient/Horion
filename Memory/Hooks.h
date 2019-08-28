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
#include "../Directx/Directx.h"
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
	static HRESULT __stdcall d3d11_present(IDXGISwapChain * pSwapChain, UINT SyncInterval, UINT Flags);
	static __int64 __fastcall setupAndRender(C_UIScene* uiscene, __int64 screencontext);
	static __int64 __fastcall uiscene_render(C_UIScene* uiscene, __int64 screencontext);
	static __int64 __fastcall renderText(__int64 a1, C_MinecraftUIRenderContext* yote);
	static char* __fastcall I8n_get(void*, char*);
	static float* Dimension_getFogColor(__int64, float* color, float brightness);
	static void ChestBlockActor_tick(C_ChestBlockActor*, void* a);
	static void Actor_lerpMotion(C_Entity* _this, vec3_t);
	static signed int AppPlatform_getGameEdition(__int64 _this);
	static void pleaseAutoComplete(__int64 _this, __int64 a2, TextHolder* text, int a4);
	static void sendToServer(C_LoopbackPacketSender* a, C_Packet* packet);
	static float LevelRendererPlayer_getFov(__int64 _this, float a2, bool a3);
	static bool Mob_isAlive(C_Entity* a1);
	static void MultiLevelPlayer_tick(C_EntityList* entityList);
	static void GameMode_startDestroyBlock(C_GameMode* _this, vec3_ti* a2, uint8_t face,void* a4,void* a5);
	static void HIDController_keyMouse(C_HIDController* _this, void* a2, void* a3);
	static int __fastcall BlockLegacy_getRenderLayer(C_BlockLegacy* a1);
	static BYTE* __fastcall BlockLegacy_getLightEmission(C_BlockLegacy* _this,BYTE* a2);
	static __int64 LevelRenderer_renderLevel(__int64 _this, __int64 a2, __int64 a3);
	static void __fastcall clickFunc(__int64 a1, char a2, char a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7, char a8);
	static __int64 __fastcall MoveInputHandler_tick(C_MoveInputHandler* _this,C_Entity* a2);
	static __int64 __fastcall chestScreenController__tick(C_ChestScreenController* _this);
	static __int64 __fastcall fullBright(__int64 a1);
	static bool __fastcall Actor__isInWater(C_Entity* _this);
	static void __fastcall jumpPower(C_Entity* _this, float a2);
	static __int64 __fastcall MinecraftGame__onAppSuspended(__int64 _this);
	static void __fastcall ladderUp(C_Entity* _this);
	static void __fastcall Actor__startSwimming(C_Entity* _this);
	static void __fastcall RakNetInstance__tick(C_RakNetInstance* _this);
	static __int64 __fastcall inventoryScreen__tick(C_CraftingScreenController* _this, __int64 a2);
	static float __fastcall GameMode__getPickRange(C_GameMode* _this, __int64 a2,char a3);
	 
	std::unique_ptr<FuncHook> gameMode_tickHook;
	std::unique_ptr<FuncHook> survivalMode_tickHook;
	std::unique_ptr<FuncHook> chatScreen_sendMessageHook;
	std::unique_ptr<FuncHook> d3d11_presentHook;
	std::unique_ptr<FuncHook> renderTextHook;
	std::unique_ptr<FuncHook> setupRenderHook;
	std::unique_ptr<FuncHook> uiscene_RenderHook;
	std::unique_ptr<FuncHook> I8n_getHook;
	std::unique_ptr<FuncHook> Dimension_getFogColorHook;
	std::unique_ptr<FuncHook> ChestBlockActor_tickHook;
	std::unique_ptr<FuncHook> Actor_lerpMotionHook;
	std::unique_ptr<FuncHook> AppPlatform_getGameEditionHook;
	std::unique_ptr<FuncHook> autoComplete_Hook;
	std::unique_ptr<FuncHook> sendToServerHook;
	std::unique_ptr<FuncHook> levelRendererPlayer_getFovHook;
	std::unique_ptr<FuncHook> mob_isAliveHook;
	std::unique_ptr<FuncHook> MultiLevelPlayerHook;
	std::unique_ptr<FuncHook> GameMode_startDestroyHook;
	std::unique_ptr<FuncHook> HIDController_keyMouseHook;
	std::unique_ptr<FuncHook> BlockLegacy_getRenderLayerHook;
	std::unique_ptr<FuncHook> BlockLegacy_getLightEmissionHook;
	std::unique_ptr<FuncHook> LevelRenderer_renderLevelHook;
	std::unique_ptr<FuncHook> Player_isUsingItemHook;
	std::unique_ptr<FuncHook> clickHook;
	std::unique_ptr<FuncHook> MoveInputHandler_tickHook;
	std::unique_ptr<FuncHook> chestScreenController__tickHook;
	std::unique_ptr<FuncHook> fullBright__Hook;
	std::unique_ptr<FuncHook> Actor__isInWaterHook;
	std::unique_ptr<FuncHook> jumpPowerHook;
	std::unique_ptr<FuncHook> MinecraftGame__onAppSuspendedHook;
	std::unique_ptr<FuncHook> ladderUpHook;
	std::unique_ptr<FuncHook> Actor__startSwimmingHook;
	std::unique_ptr<FuncHook> RakNetInstance__tickHook;
	std::unique_ptr<FuncHook> inventoryScreen__tickHook;
	std::unique_ptr<FuncHook> GameMode__getPickRangeHook;


	typedef __int64(__fastcall* LevelRenderer_renderLevel_t)(__int64 _this, __int64 a2, __int64 a3);
	typedef bool(__fastcall* mob_isAlive_T)(C_Entity* _this);
	typedef float(__fastcall* getFov_t)(__int64 _this, float a2, bool a3);
	typedef void(__fastcall* autoComplete_t)(__int64 a1, __int64 a2, TextHolder* text, int a4);
	typedef signed int(__fastcall* AppPlatform_getGameEdition_t)(__int64);
	typedef void(__fastcall* Actor_lerpMotion_t)(C_Entity*, vec3_t);
	typedef float*(__fastcall* Dimension_getFogColor_t)(__int64, float*, float);
	typedef char*(__fastcall* I8n_get_t)(void*, char*);
	typedef __int64(__fastcall* uirender_t)(C_UIScene*, __int64);
	typedef __int64(__fastcall* setupRender_t)(C_UIScene*, __int64);
	typedef __int64(__fastcall* renderText_t)(__int64, C_MinecraftUIRenderContext*);
	typedef void(__fastcall* GameMode_tick_t)(C_GameMode* _this);
	typedef void(__fastcall* SurvivalMode_tick_t)(C_GameMode* _this);
	typedef void(__fastcall* ChatScreen_sendChatMessage_t)(void* _this);
	typedef HRESULT(__stdcall* d3d11_present_t)(IDXGISwapChain * pSwapChain, UINT SyncInterval, UINT Flags);
	typedef void(__fastcall* ChestBlockActor_tick_t)(void* _this,void*);
	typedef void(__fastcall* sendToServer_tick_t)(C_LoopbackPacketSender* _this, C_Packet* packet);
	typedef void(__fastcall* MultiLevelPlayer_tick_t)(C_EntityList* entityList);
	typedef void(__fastcall* GameMode_startDestroyBlock_t)(C_GameMode* _this, vec3_ti* a2, uint8_t face, void* a4, void* a5);
	typedef void(__fastcall* HIDController_keyMouse_t)(C_HIDController* _this,void* a2, void* a3);
	typedef int(__fastcall *BlockLegacy_getRenderLayer_t)(C_BlockLegacy* _this);
	typedef BYTE*(__fastcall *BlockLegacy_getLightEmission_t)(C_BlockLegacy* _this,BYTE* a2);
	typedef __int64(__fastcall *clickFunc_t)(__int64 a1, char a2, char a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7, char a8);
	typedef __int64(__fastcall *MoveInputHandler_tick_t)(C_MoveInputHandler* _this, C_Entity* a2);
	typedef __int64(__fastcall *chestScreenController__tick_t)(C_ChestScreenController* _this);
	typedef __int64(__fastcall *fullbright_t)(__int64 a1);
	typedef bool(__fastcall * Actor__isInWater_t)(C_Entity * _this);
	typedef __int64(__fastcall* jumpPower_t)(C_Entity* _this, float a2);
	typedef __int64(__fastcall* MinecraftGame__onAppSuspended_t)(__int64 _this);
	typedef void(__fastcall* ladderUp_t)(C_Entity* _this);
	typedef void(__fastcall* Actor__startSwimming_t)(C_Entity* _this);
	typedef void(__fastcall* RakNetInstance__tick_t)(C_RakNetInstance* _this);
	typedef __int64(__fastcall* inventoryScreen__tick_t)(C_CraftingScreenController* _this, __int64 a2);
	typedef float(__fastcall* GameMode__getPickRange_t)(C_GameMode* _this, __int64 a2, char a3);
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