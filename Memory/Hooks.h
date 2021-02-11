#pragma once

#ifndef PERFORMANCE_TEST
//#define PERFORMANCE_TEST
#endif

#include "../Horion/Command/CommandMgr.h"
#include "../Horion/Config/ConfigManager.h"
#include "../Horion/DrawUtils.h"
#include "../Horion/ImmediateGui.h"
#include "../Horion/Menu/ClickGui.h"
#include "../Horion/Menu/TabGui.h"
#include "../Horion/Module/ModuleManager.h"
#include "../SDK/CBlockLegacy.h"
#include "../SDK/CChestBlockActor.h"
#include "../SDK/CGameMode.h"
#include "../SDK/CMinecraftUIRenderContext.h"
#include "../SDK/CMoveInputHandler.h"
#include "../SDK/CRakNetInstance.h"
#include "../SDK/CUIScene.h"
#include "../SDK/TextHolder.h"
#include "../SDK/CCamera.h"
#include "../Utils/TextFormat.h"
#include "../Utils/SkinUtil.h"
#include "../resource.h"
#include "GameData.h"
#include "MinHook.h"
//#include "../Horion/Game/Game.h"

#include <intrin.h>
#include <thread>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "../include/d3dx11async.h"

class VMTHook;
class FuncHook;

struct CoolSkinData {
	TextHolder unknown;
	TextHolder unknown2;
	TextHolder skinResourcePatch;  // 0x040
	TextHolder geometryName; // 0x060 "geometry.humanoid.custom"
	unsigned char gap2[0x40];      // 0x080
	void* startAnimatedFrames;     // 0x0C0
	void* endAnimatedFrames;       // 0x0C8
	unsigned char gap3[0x8];      // 0x0D0
	TextHolder geometryData;		// 0x0D8
	TextHolder skinAnimationData;  // 0x0F8
	unsigned char gap4[0x20];      // 0x118
	bool isPremiumSkin;            // 0x138
	bool isPersonaSkin;
	bool isCapeOnClassicSkin;
	void* startPersonaPieces;
	void* endPersonaPieces;
	unsigned char gap5[0x8];  // 0x150
	TextHolder armSize;       // 0x158
	unsigned char gap6[0x8];  // 0x178
	void* startPieces;
};

class Hooks {
private:
	bool shouldRender = true;
	char currentScreenName[100];
		 
public:
	std::vector<std::shared_ptr<FuncHook>> lambdaHooks;

	static void Init();
	static void Restore();
	static void Enable();

private:
	static void GameMode_tick(C_GameMode* _this);
	static void SurvivalMode_tick(C_GameMode* _this);
	static void ChatScreenController_sendChatMessage(uint8_t* _this);
	static __int64 UIScene_setupAndRender(C_UIScene* uiscene, __int64 screencontext);
	static __int64 UIScene_render(C_UIScene* uiscene, __int64 screencontext);
	static __int64 RenderText(__int64 a1, C_MinecraftUIRenderContext* renderCtx);
	static float* Dimension_getFogColor(__int64, float* color, __int64 brightness, float a4);
	static float Dimension_getTimeOfDay(__int64, int a2, float a3);
	static float Dimension_getSunIntensity(__int64, float a2, vec3_t* a3, float a4);
	static void ChestBlockActor_tick(C_ChestBlockActor*, void* a);
	static void Actor_lerpMotion(C_Entity* _this, vec3_t);
	static int AppPlatform_getGameEdition(__int64 _this);
	static void PleaseAutoComplete(__int64 _this, __int64 a2, TextHolder* text, int a4);
	static void LoopbackPacketSender_sendToServer(C_LoopbackPacketSender* a, C_Packet* packet);
	static float LevelRendererPlayer_getFov(__int64 _this, float a2, bool a3);
	static void MultiLevelPlayer_tick(C_EntityList* entityList);
	static void GameMode_startDestroyBlock(C_GameMode* _this, vec3_ti* a2, uint8_t face, void* a4, void* a5);
	static void HIDController_keyMouse(C_HIDController* _this, void* a2, void* a3);
	static int BlockLegacy_getRenderLayer(C_BlockLegacy* a1);
	static __int8* BlockLegacy_getLightEmission(C_BlockLegacy* _this, __int8* a2);
	static __int64 LevelRenderer_renderLevel(__int64 _this, __int64 a2, __int64 a3);
	static void ClickFunc(__int64 a1, char a2, char a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7, char a8);
	static __int64 MoveInputHandler_tick(C_MoveInputHandler* _this, C_Entity* a2);
	static __int64 ChestScreenController_tick(C_ChestScreenController* _this);
	static float GetGamma(uintptr_t* a1);
	static bool Actor_isInWater(C_Entity* _this);
	static void JumpPower(C_Entity* _this, float a2);
	static __int64 MinecraftGame_onAppSuspended(__int64 _this);
	static void Actor_ascendLadder(C_Entity* _this);
	static void Actor_startSwimming(C_Entity* _this);
	static void RakNetInstance_tick(C_RakNetInstance* _this, __int64 a2, __int64 a3);
	static float GameMode_getPickRange(C_GameMode* _this, __int64 a2, char a3);
	static __int64 GameMode_attack(C_GameMode* _this, C_Entity*);
	static __int64 ConnectionRequest_create(__int64 _this, __int64 privateKeyManager, void* a3, TextHolder* selfSignedId, TextHolder* serverAddress, __int64 clientRandomId, TextHolder* skinId, SkinData* skinData, __int64 capeData, CoolSkinData* coolSkinStuff, TextHolder* deviceId, int inputMode, int uiProfile, int guiScale, TextHolder* languageCode, bool sendEduModeParams, TextHolder* tenantId, __int64 unused, TextHolder* platformUserId, TextHolder* thirdPartyName, bool thirdPartyNameOnly, TextHolder* platformOnlineId, TextHolder* platformOfflineId, TextHolder* capeId);
	static void InventoryTransactionManager_addAction(C_InventoryTransactionManager* a1, C_InventoryAction* a2);
	static __int64 PaintingRenderer__render(__int64 _this, __int64 a2, __int64 a3);
	static bool DirectoryPackAccessStrategy__isTrusted(__int64 _this);
	static bool ReturnTrue(__int64 _this);
	static __int64 SkinRepository___loadSkinPack(__int64 _this, __int64 pack, __int64 a3);
	static GamerTextHolder* toStyledString(__int64 strIn, GamerTextHolder* strOut);
	static __int64 prepFeaturedServers(__int64 a1);
	static __int64 prepFeaturedServersFirstTime(__int64 a1, __int64 a2);
	static HRESULT swapChain__present(IDXGISwapChain* chain, UINT syncInterval, UINT flags);
	static __int64 InGamePlayScreen___renderLevel(__int64 playScreen, __int64 a2, __int64 a3);
	static HRESULT swapChain__ResizeBuffers(IDXGISwapChain* chain, UINT bufferCount, UINT Width, UINT Height,DXGI_FORMAT Newformat,UINT SwapChainFlags);
	static __int64 Cube__compile(__int64 a1, __int64 a2);
	static void LocalPlayer__updateFromCamera(__int64 a1, C_Camera* a2);
	static bool Mob__isImmobile(C_Entity*);
	static void InventoryTransactionManager__addAction(C_InventoryTransactionManager*, C_InventoryAction &);
	static void LevelRendererPlayer__renderNameTags(__int64 a1, __int64 a2,TextHolder* name, __int64 a4);

	std::unique_ptr<FuncHook> GameMode_tickHook;
	std::unique_ptr<FuncHook> SurvivalMode_tickHook;
	std::unique_ptr<FuncHook> ChatScreenController_sendChatMessageHook;
	std::unique_ptr<FuncHook> UIScene_setupAndRenderHook;
	std::unique_ptr<FuncHook> UIScene_renderHook;
	std::unique_ptr<FuncHook> RenderTextHook;
	std::unique_ptr<FuncHook> Dimension_getFogColorHook;
	std::unique_ptr<FuncHook> Dimension_getTimeOfDayHook;
	std::unique_ptr<FuncHook> Dimension_getSunIntensityHook;
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
	std::unique_ptr<FuncHook> Actor_ascendLadderHook;
	std::unique_ptr<FuncHook> Actor_startSwimmingHook;
	std::unique_ptr<FuncHook> RakNetInstance_tickHook;
	std::unique_ptr<FuncHook> GameMode_getPickRangeHook;
	std::unique_ptr<FuncHook> GameMode_attackHook;
	std::unique_ptr<FuncHook> ConnectionRequest_createHook;
	std::unique_ptr<FuncHook> InventoryTransactionManager_addActionHook;
	std::unique_ptr<FuncHook> PaintingRenderer__renderHook;
	std::unique_ptr<FuncHook> DirectoryPackAccessStrategy__isTrustedHook;
	std::unique_ptr<FuncHook> ZipPackAccessStrategy__isTrustedHook;
	std::unique_ptr<FuncHook> SkinRepository___checkSignatureFileInPack;
	std::unique_ptr<FuncHook> SkinRepository___loadSkinPackHook;
	std::unique_ptr<FuncHook> toStyledStringHook;
	std::unique_ptr<FuncHook> prepFeaturedServersHook;
	std::unique_ptr<FuncHook> prepFeaturedServersFirstTimeHook;
	std::unique_ptr<FuncHook> swapchain__presentHook;
	std::unique_ptr<FuncHook> InGamePlayScreen___renderLevelHook;
	std::unique_ptr<FuncHook> swapchain__resizeBuffersHook;
	std::unique_ptr<FuncHook> cube__compileHook;
	std::unique_ptr<FuncHook> LocalPlayer__updateFromCameraHook;
	std::unique_ptr<FuncHook> Mob__isImmobileHook;
	std::unique_ptr<FuncHook> InventoryTransactionManager__addActionHook;
	std::unique_ptr<FuncHook> LevelRendererPlayer__renderNameTagsHook;
};

extern Hooks g_Hooks;

class FuncHook {
public:
	void* funcPtr;
	void* funcReal;

	FuncHook(void* func, void* hooked) {
		funcPtr = func;

		MH_STATUS ret = MH_CreateHook(func, hooked, &funcReal);
		if (ret == MH_OK && (__int64)func > 10) {
		} else
			logF("MH_CreateHook = %i", ret);
	};

	FuncHook(uintptr_t func, void* hooked) {
		funcPtr = reinterpret_cast<void*>(func);

		MH_STATUS ret = MH_CreateHook(funcPtr, hooked, &funcReal);
		if (ret == MH_OK && (__int64)funcPtr > 10) {
		} else
			logF("MH_CreateHook = %i", ret);
	};

	void enableHook(bool enable = true) {
		if (funcPtr != nullptr) {
			int ret = enable ? MH_EnableHook(funcPtr) : MH_DisableHook(funcPtr);
			if (ret != MH_OK)
				logF("MH_EnableHook = %i", ret);
		}else
			logF("enableHook() called with nullptr func!");
		
	}

	~FuncHook() {
		Restore();
	}

	void Restore() {
		if (funcPtr != nullptr)
			MH_DisableHook(funcPtr);
	}

	template <typename TRet, typename... TArgs>
	inline auto* GetFastcall() {
		using Fn = TRet(__fastcall*)(TArgs...);
		return reinterpret_cast<Fn>(funcReal);
	};
};