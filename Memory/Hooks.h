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
	static void		GameMode_tick(C_GameMode* _this);
	static void		SurvivalMode_tick(C_GameMode* _this);
	static void		ChatScreenController_sendChatMessage(uint8_t* _this);
	static __int64	UIScene_setupAndRender(C_UIScene* uiscene, __int64 screencontext);
	static __int64	UIScene_render(C_UIScene* uiscene, __int64 screencontext);
	static __int64	RenderText(__int64 a1, C_MinecraftUIRenderContext* renderCtx);
	static float*	Dimension_getFogColor(__int64, float* color, float brightness);
	static void		ChestBlockActor_tick(C_ChestBlockActor*, void* a);
	static void		Actor_lerpMotion(C_Entity* _this, vec3_t);
	static int		AppPlatform_getGameEdition(__int64 _this);
	static void		PleaseAutoComplete(__int64 _this, __int64 a2, TextHolder* text, int a4);
	static void		LoopbackPacketSender_sendToServer(C_LoopbackPacketSender* a, C_Packet* packet);
	static float	LevelRendererPlayer_getFov(__int64 _this, float a2, bool a3);
	static void		MultiLevelPlayer_tick(C_EntityList* entityList);
	static void		GameMode_startDestroyBlock(C_GameMode* _this, vec3_ti* a2, uint8_t face, void* a4, void* a5);
	static void		HIDController_keyMouse(C_HIDController* _this, void* a2, void* a3);
	static int		BlockLegacy_getRenderLayer(C_BlockLegacy* a1);
	static __int8*	BlockLegacy_getLightEmission(C_BlockLegacy* _this, __int8* a2);
	static __int64	LevelRenderer_renderLevel(__int64 _this, __int64 a2, __int64 a3);
	static void		ClickFunc(__int64 a1, char a2, char a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7, char a8);
	static __int64	MoveInputHandler_tick(C_MoveInputHandler* _this, C_Entity* a2);
	static __int64	ChestScreenController_tick(C_ChestScreenController* _this);
	static __int64	GetGamma(__int64 a1);
	static bool		Actor_isInWater(C_Entity* _this);
	static void		JumpPower(C_Entity* _this, float a2);
	static __int64	MinecraftGame_onAppSuspended(__int64 _this);
	static void		Actor_ladderUp(C_Entity* _this);
	static void		Actor_startSwimming(C_Entity* _this);
	static void		RakNetInstance_tick(C_RakNetInstance* _this);
	static float	GameMode_getPickRange(C_GameMode* _this, __int64 a2, char a3);
	static void		InventoryTransactionManager_addAction(C_InventoryTransactionManager* a1, C_InventoryAction* a2);

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
	std::unique_ptr<FuncHook> Actor_ladderUpHook;
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
		
		MH_STATUS ret = MH_CreateHook(func, hooked, &funcReal);
		if (ret == MH_OK && (__int64)func > 10) {
			
		}else
			logF("MH_CreateHook = %i", ret);
	};

	FuncHook(uintptr_t func, void* hooked) {
		funcPtr = reinterpret_cast<void*>(func);

		MH_STATUS ret = MH_CreateHook(funcPtr, hooked, &funcReal);
		if (ret == MH_OK && (__int64)funcPtr > 10) {

		}
		else
			logF("MH_CreateHook = %i", ret);
	};

	void enableHook(bool enable = true) {
		int ret = enable ? MH_EnableHook(funcPtr) : MH_DisableHook(funcPtr);
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
	inline auto* GetFastcall()
	{
		using Fn = TRet(__fastcall*)(TArgs...);
		return reinterpret_cast<Fn>(funcReal);
	};
};
