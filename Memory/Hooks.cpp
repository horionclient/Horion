#include "Hooks.h"
#include "../Directx/Directx.h"

Hooks    g_Hooks;
bool isTicked = false;
//#define TEST_DEBUG

void Hooks::Init()
{
	logF("Setting up Hooks...");

	static uintptr_t** GameModeVtable = 0x0;
	if (GameModeVtable == 0x0) {
		uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 01 33 D2 48 C7 41 ??");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		GameModeVtable = reinterpret_cast<uintptr_t * *>(sigOffset + offset + /*length of instruction*/ 7);
		if (GameModeVtable == 0x0 || sigOffset == 0x0)
			logF("C_GameMode signature not working!!!");
		else
		{
			g_Hooks.gameMode_tickHook = std::make_unique<FuncHook>(GameModeVtable[9], Hooks::GameMode_tick);
			g_Hooks.gameMode_tickHook->init();

			g_Hooks.GameMode_startDestroyHook = std::make_unique<FuncHook>(GameModeVtable[1], Hooks::GameMode_startDestroyBlock);
			g_Hooks.GameMode_startDestroyHook->init();

			g_Hooks.GameMode__getPickRangeHook = std::make_unique<FuncHook>(GameModeVtable[10], Hooks::GameMode__getPickRange);
			g_Hooks.GameMode__getPickRangeHook->init();
		}
	}

	// SurvivalMode::tick Sig
	void* surv_tick = reinterpret_cast<void*>(Utils::FindSignature("48 8B C4 55 48 8D 68 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 10 48 89 70 18 48 89 78 20 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 48 8B F9 8B 41 ??"));
	g_Hooks.survivalMode_tickHook = std::make_unique<FuncHook>(surv_tick, Hooks::SurvivalMode_tick);
	g_Hooks.survivalMode_tickHook->init();

	// ChatScreenController::_sendChatMessage
	// 40 57 48 83 EC 20 48 83 B9 ?? ?? ?? ?? 00 48 8B F9 0F 85
	// ^^ 
	void* _sendChatMessage = reinterpret_cast<void*>(Utils::FindSignature("40 57 48 83 EC 20 48 83 B9 ?? ?? ?? ?? 00 48 8B F9 0F 85"));
	g_Hooks.chatScreen_sendMessageHook = std::make_unique<FuncHook>(_sendChatMessage, Hooks::ChatScreenController_sendChatMessage);
	g_Hooks.chatScreen_sendMessageHook->init();

	//IDXGISwapChain::present;
	// using vtable found with dummy thing
	void** swapChainVtable = static_cast<void**>(getSwapChain());
	if (swapChainVtable != nullptr) {
		void* presentFunc = swapChainVtable[8];
		g_Hooks.d3d11_presentHook = std::make_unique<FuncHook>(presentFunc, Hooks::d3d11_present);
		g_Hooks.d3d11_presentHook->init();
	}

	void* _shit = reinterpret_cast<void*>(Utils::FindSignature("48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 ?? 0F 29  70 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 8985 ?? ?? ?? ?? 48 89 54 24"));
	g_Hooks.renderTextHook = std::make_unique<FuncHook>(_shit, Hooks::renderText);
	g_Hooks.renderTextHook->init();

	void* setupRender = reinterpret_cast<void*>(Utils::FindSignature("40 57 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B DA 48 8B F9 33 D2 ?? ?? ?? ?? ?? ?? 48 8D 4C 24 30 E8 ?? ?? ?? ?? 4C 8B CF 4C 8B C3 48 8B 57 ?? 48 8D 4C 24 ??"));
	g_Hooks.setupRenderHook = std::make_unique<FuncHook>(setupRender, Hooks::setupAndRender);
	g_Hooks.setupRenderHook->init();

	void* render = reinterpret_cast<void*>(Utils::FindSignature("40 56 57 41 56 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B FA 48 8B D9 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 30 41 8B 04 36 39 05 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 33 C0"));
	g_Hooks.uiscene_RenderHook = std::make_unique<FuncHook>(render, Hooks::uiscene_render);
	g_Hooks.uiscene_RenderHook->init();

	void* fogColorFunc = reinterpret_cast<void*>(Utils::FindSignature("0F 28 C2 C7 42 0C 00 00 80 3F F3"));
	g_Hooks.Dimension_getFogColorHook = std::make_unique<FuncHook>(fogColorFunc, Hooks::Dimension_getFogColor);
	g_Hooks.Dimension_getFogColorHook->init();

	void* ChestTick = reinterpret_cast<void*>(Utils::FindSignature("40 53 57 48 83 EC ?? 48 8B 41 ?? 48 8B FA 48 89 6C 24 ?? 48 8B D9 4C 89 74 24 ?? 48 85 C0 75 10 48 8D 51 ?? 48 8B CF E8 ?? ?? ?? ?? 48 89 43 ?? FF 43 ?? 48 85 C0"));
	g_Hooks.ChestBlockActor_tickHook = std::make_unique <FuncHook>(ChestTick, Hooks::ChestBlockActor_tick);
	g_Hooks.ChestBlockActor_tickHook->init();

	void* lerpFunc = reinterpret_cast<void*>(Utils::FindSignature("8B 02 89 81 ?? 0E ?? ?? 8B 42 04 89 81 ?? ?? ?? ?? 8B 42 08 89 81 ?? ?? ?? ?? C3"));
	g_Hooks.Actor_lerpMotionHook = std::make_unique <FuncHook>(lerpFunc, Hooks::Actor_lerpMotion);
	g_Hooks.Actor_lerpMotionHook->init();

	void* getGameEdition = reinterpret_cast<void*>(Utils::FindSignature("8B 91 ?? ?? ?? ?? 85 D2 74 1C 83 EA 01"));
	g_Hooks.AppPlatform_getGameEditionHook = std::make_unique <FuncHook>(getGameEdition, Hooks::AppPlatform_getGameEdition);
	g_Hooks.AppPlatform_getGameEditionHook->init();

	void* autoComplete = reinterpret_cast<void*>(Utils::FindSignature("40 55 53 56 57 41 56 48 8D 6C 24 C9 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 45 8B F1 49 8B F8 48 8B F2 48 8B D9 48 89 55 ?? 0F 57 C0 F3 0F 7F 45 ?? 48 8B 52 ?? 48 85 D2"));
	g_Hooks.autoComplete_Hook = std::make_unique <FuncHook>(autoComplete, Hooks::pleaseAutoComplete);
	g_Hooks.autoComplete_Hook->init();

	void* sendtoServer = reinterpret_cast<void*>(Utils::FindSignature("48 89 5C 24 08 57 48 ?? ?? ?? ?? ?? ?? 0F B6 41 ?? 48 8B FA 88 42 ?? 48 8D 54 24 ?? 48 8B 59 ?? 48 8B CB E8 ?? ?? ?? ?? 48 8B D0 45 33 C9"));
	g_Hooks.sendToServerHook = std::make_unique <FuncHook>(sendtoServer, Hooks::sendToServer);
	g_Hooks.sendToServerHook->init();

	void* getFov = reinterpret_cast<void*>(Utils::FindSignature("40 53 48 83 EC ?? 0F 29 74 24 ?? 0F 29 7C 24 ?? 44 0F 29"));
	g_Hooks.levelRendererPlayer_getFovHook = std::make_unique<FuncHook>(getFov, Hooks::LevelRendererPlayer_getFov);
	g_Hooks.levelRendererPlayer_getFovHook->init();

	void* mob_isAlive = reinterpret_cast<void*>(Utils::FindSignature("48 83 EC ?? 80 B9 ?? ?? ?? ?? 00 75 ?? 48 8B 01 48 8D"));
	g_Hooks.mob_isAliveHook = std::make_unique<FuncHook>(mob_isAlive, Hooks::Mob_isAlive);
	g_Hooks.mob_isAliveHook->init();

	void* tick_entityList = reinterpret_cast<void*>(Utils::FindSignature("40 53 48 83 EC 20 48 8B D9 E8 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 49 8B C8 4D 85 C0 75 07"));
	g_Hooks.MultiLevelPlayerHook = std::make_unique<FuncHook>(tick_entityList, Hooks::MultiLevelPlayer_tick);
	g_Hooks.MultiLevelPlayerHook->init();

	void* keyMouseFunc = reinterpret_cast<void*>(Utils::FindSignature("40 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 70 48 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 F0 49 8B F0 48 8B F9 45 33 ED 41 8B DD 89 5D EC 49 8B C8"));
	g_Hooks.HIDController_keyMouseHook = std::make_unique<FuncHook>(keyMouseFunc, Hooks::HIDController_keyMouse);
	g_Hooks.HIDController_keyMouseHook->init();

	static uintptr_t** blockLegacyVtable = 0x0;
	if (blockLegacyVtable == 0x0) {
		uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 01 4C 39 72");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		blockLegacyVtable = reinterpret_cast<uintptr_t * *>(sigOffset + offset + /*length of instruction*/ 7);
		if (blockLegacyVtable == 0x0 || sigOffset == 0x0)
			logF("C_BlockLegacy signature not working!!!");
		else
		{
			g_Hooks.BlockLegacy_getRenderLayerHook = std::make_unique<FuncHook>(blockLegacyVtable[115], Hooks::BlockLegacy_getRenderLayer);
			g_Hooks.BlockLegacy_getRenderLayerHook->init();

			g_Hooks.BlockLegacy_getLightEmissionHook = std::make_unique<FuncHook>(blockLegacyVtable[14], Hooks::BlockLegacy_getLightEmission);
			g_Hooks.BlockLegacy_getLightEmissionHook->init();
		}
	}

	static uintptr_t** localPlayerVtable = 0x0;
	if (localPlayerVtable == 0x0) {
		uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 45 00 49 8D 8D");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		localPlayerVtable = reinterpret_cast<uintptr_t * *>(sigOffset + offset + /*length of instruction*/ 7);
		if (localPlayerVtable == 0x0 || sigOffset == 0x0)
			logF("C_LocalPlayer signature not working!!!");
		else
		{
			g_Hooks.Actor__isInWaterHook = std::make_unique<FuncHook>(localPlayerVtable[59], Hooks::Actor__isInWater);
			g_Hooks.Actor__isInWaterHook->init();

			g_Hooks.Actor__startSwimmingHook = std::make_unique<FuncHook>(localPlayerVtable[180], Hooks::Actor__isInWater);
			g_Hooks.Actor__startSwimmingHook->init();

			g_Hooks.ladderUpHook = std::make_unique<FuncHook>(localPlayerVtable[323], Hooks::ladderUp);
			g_Hooks.ladderUpHook->init();
		}
	}

	void* renderLevel = reinterpret_cast<void*>(Utils::FindSignature("40 53 56 57 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? FE FF FF FF 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 49 8B D8 48 8B FA 48 8B F1 33 D2"));
	g_Hooks.LevelRenderer_renderLevelHook = std::make_unique<FuncHook>(renderLevel, Hooks::LevelRenderer_renderLevel);
	g_Hooks.LevelRenderer_renderLevelHook->init();

	void* clickHook = reinterpret_cast<void*>(Utils::FindSignature("48 8B C4 48 89 58 ?? 48 89 68 ?? 48 89 70 ?? 57 41 54 41 55 41 56 41 57 48 83 EC 60 44 ?? ?? ?? ?? ?? ?? ?? ?? 33 F6"));
	g_Hooks.clickHook = std::make_unique<FuncHook>(clickHook, Hooks::clickFunc);
	g_Hooks.clickHook->init();

	void* MoveInputHandlerTick = reinterpret_cast<void*>(Utils::FindSignature("48 8B C4 56 57 41 54 41 56 41 57 48 83 EC 50 48 ?? ?? ?? ?? ?? ?? ?? 48 89 58 ?? 48 89 68 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B FA 48 8B D9 33 C0"));
	g_Hooks.MoveInputHandler_tickHook = std::make_unique<FuncHook>(MoveInputHandlerTick, Hooks::MoveInputHandler_tick);
	g_Hooks.MoveInputHandler_tickHook->init();

	void* chestScreenControllerTick = reinterpret_cast<void*>(Utils::FindSignature("48 89 5C 24 08 57 48 83 EC 20 48 8B F9 E8 ?? ?? ?? ?? 48 8B 17 48 8B CF 8B D8 FF 92 ?? ?? ?? ?? 84 C0 74 31"));
	g_Hooks.chestScreenController__tickHook = std::make_unique<FuncHook>(chestScreenControllerTick, Hooks::chestScreenController__tick);
	g_Hooks.chestScreenController__tickHook->init();

	void* fullbright = reinterpret_cast<void*>(Utils::FindSignature("40 57 48 83 EC 40 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 89 74 24 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 33 C0 48 89 44 24 ?? 48 89 44 24 ?? 48 8B 01 48 8D 54 24 ??"));
	g_Hooks.fullBright__Hook = std::make_unique<FuncHook>(fullbright, Hooks::fullBright);
	g_Hooks.fullBright__Hook->init();

	void* jump = reinterpret_cast<void*>(Utils::FindSignature("40 57 48 83 EC 40 48 8B 01 48 8B F9 FF 50 ?? 8B 08 89 ?? ?? ?? ?? ?? 8B 48 ?? 89"));
	g_Hooks.jumpPowerHook = std::make_unique<FuncHook>(jump, Hooks::jumpPower);
	g_Hooks.jumpPowerHook->init();

	void* onAppSuspended = reinterpret_cast<void*>(Utils::FindSignature("48 8B C4 57 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 58 ?? 48 89 68 ?? 48 89 70 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B F1 ?? ?? ?? ?? ?? ?? ?? 48 85 C9"));
	g_Hooks.MinecraftGame__onAppSuspendedHook = std::make_unique<FuncHook>(onAppSuspended, Hooks::MinecraftGame__onAppSuspended);
	g_Hooks.MinecraftGame__onAppSuspendedHook->init();

	void* RakNetInstance__tick = reinterpret_cast<void*>(Utils::FindSignature("48 8B C4 55 57 41 54 41 56 41 57 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 58 ?? 48 89 70 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? 48 8B F9 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 45 33 E4 41 8B F4"));
	g_Hooks.RakNetInstance__tickHook = std::make_unique<FuncHook>(RakNetInstance__tick, Hooks::RakNetInstance__tick);
	g_Hooks.RakNetInstance__tickHook->init();

#ifdef TEST_DEBUG
	void* addAction = reinterpret_cast<void*>(Utils::FindSignature("40 55 56 57 41 56 41 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 8B EA 4C 8B F1 4C 8B C2 48 8B 51 ?? 48 8B 49 ?? E8"));
	g_Hooks.InventoryTransactionManager__addActionHook = std::make_unique<FuncHook>(addAction, Hooks::InventoryTransactionManager__addAction);
	g_Hooks.InventoryTransactionManager__addActionHook->init();
#endif
}

void Hooks::Restore()
{
	g_Hooks.gameMode_tickHook->Restore();
	g_Hooks.chatScreen_sendMessageHook->Restore();
	g_Hooks.d3d11_presentHook->Restore();
	g_Hooks.renderTextHook->Restore();
	//g_Hooks.I8n_getHook->Restore();
	g_Hooks.AppPlatform_getGameEditionHook->Restore();
	g_Hooks.autoComplete_Hook->Restore();
	g_Hooks.levelRendererPlayer_getFovHook->Restore();
	g_Hooks.ChestBlockActor_tickHook->Restore();
	g_Hooks.sendToServerHook->Restore();
	g_Hooks.MultiLevelPlayerHook->Restore();
	g_Hooks.mob_isAliveHook->Restore();
	g_Hooks.GameMode_startDestroyHook->Restore();
	g_Hooks.HIDController_keyMouseHook->Restore();
	g_Hooks.BlockLegacy_getRenderLayerHook->Restore();
	g_Hooks.LevelRenderer_renderLevelHook->Restore();
	g_Hooks.BlockLegacy_getLightEmissionHook->Restore();
	g_Hooks.clickHook->Restore();
	g_Hooks.MoveInputHandler_tickHook->Restore();
	g_Hooks.chestScreenController__tickHook->Restore();
	g_Hooks.fullBright__Hook->Restore();
	g_Hooks.Actor__isInWaterHook->Restore();
	g_Hooks.jumpPowerHook->Restore();
	g_Hooks.MinecraftGame__onAppSuspendedHook->Restore();
	g_Hooks.ladderUpHook->Restore();
	g_Hooks.RakNetInstance__tickHook->Restore();
	g_Hooks.GameMode__getPickRangeHook->Restore();
	g_Hooks.InventoryTransactionManager__addActionHook->Restore();
}

#ifdef TEST_DEBUG
	int ofa = 0;
#endif

void __fastcall Hooks::InventoryTransactionManager__addAction(C_InventoryTransactionManager* a1, C_InventoryAction* a2)
{
	static auto Func = g_Hooks.InventoryTransactionManager__addActionHook->GetOriginal<InventoryTransactionManager__addAction_t>();
#ifdef TEST_DEBUG
	ofa++;
	logF("rcx : %llX", a1);
	logF("ofa : %d", ofa);
	logF("type  0 : %d", a2->type);
	logF("source type  : %d", a2->sourceType);
	logF("slot : %d", a2->slot);
	logF("source Item : %llX", a2->sourceItem.item);
	logF("targetItem Item : %llX", a2->targetItem.item);
#endif
	Func(a1, a2);
}

float __fastcall Hooks::GameMode__getPickRange(C_GameMode* _this, __int64 a2, char a3)
{
	static auto oFunc = g_Hooks.GameMode__getPickRangeHook->GetOriginal<GameMode__getPickRange_t>();
	static InfiniteBlockReach* InfiniteBlockReachModule = moduleMgr->getModule<InfiniteBlockReach>();
	if (InfiniteBlockReachModule == nullptr)
		InfiniteBlockReachModule = moduleMgr->getModule<InfiniteBlockReach>();
	else if (InfiniteBlockReachModule->isEnabled()) 
		return InfiniteBlockReachModule->getBlockReach();

	return oFunc(_this, a2, a3);
}

void __fastcall Hooks::RakNetInstance__tick(C_RakNetInstance* _this)
{
	static auto oTick = g_Hooks.RakNetInstance__tickHook->GetOriginal<RakNetInstance__tick_t>();
	GameData::setRakNetInstance(_this);
	oTick(_this); // Call Original Func
}

void __fastcall Hooks::ladderUp(C_Entity* _this)
{
	static auto oFunc = g_Hooks.ladderUpHook->GetOriginal<ladderUp_t>();

	static IModule* FastLadderModule = moduleMgr->getModule<FastLadder>();
	if (FastLadderModule == nullptr)
		FastLadderModule = moduleMgr->getModule<FastLadder>();
	else if (FastLadderModule->isEnabled() && g_Data.getLocalPlayer() == _this) {
		_this->velocity.y = 0.4f;
		return;
	}
	return oFunc(_this);

}

void __fastcall Hooks::Actor__startSwimming(C_Entity* _this)
{
	static auto oFunc = g_Hooks.Actor__startSwimmingHook->GetOriginal<Actor__startSwimming_t>();

	static IModule* JesusModule = moduleMgr->getModule<Jesus>();
	if (JesusModule == nullptr)
		JesusModule = moduleMgr->getModule<Xray>();
	else if (JesusModule->isEnabled() && g_Data.getLocalPlayer() == _this) {
		return;
	}
	oFunc(_this);
}

__int64 __fastcall Hooks::MinecraftGame__onAppSuspended(__int64 _this)
{
	static auto oFunc = g_Hooks.MinecraftGame__onAppSuspendedHook->GetOriginal<MinecraftGame__onAppSuspended_t>();
	configMgr->saveConfig();
	return oFunc(_this);
}

void __fastcall Hooks::jumpPower(C_Entity* a1, float a2)
{
	static auto oFunc = g_Hooks.jumpPowerHook->GetOriginal<jumpPower_t>();
	static HighJump* HighJumpMod = moduleMgr->getModule<HighJump>();
	if (HighJumpMod == nullptr)
		HighJumpMod = moduleMgr->getModule<HighJump>();
	else if (HighJumpMod->isEnabled() && g_Data.getLocalPlayer() == a1) {
		a1->velocity.y = HighJumpMod->jumpPower;
		return;
	}
	oFunc(a1, a2);
}

bool __fastcall Hooks::Actor__isInWater(C_Entity* _this)
{
	static auto oFunc = g_Hooks.Actor__isInWaterHook->GetOriginal<Actor__isInWater_t>();

	if (g_Data.getLocalPlayer() != _this)
		return oFunc(_this);

	static AirSwim* AirSwimModule = moduleMgr->getModule<AirSwim>();
	if (AirSwimModule == nullptr)
		AirSwimModule = moduleMgr->getModule<AirSwim>();
	else if (AirSwimModule->isEnabled())
		return true;

	return oFunc(_this);
}

__int64 __fastcall Hooks::fullBright(__int64 a1)
{
	static auto oFunc = g_Hooks.fullBright__Hook->GetOriginal<fullbright_t>();

	static FullBright* fullBrightModule = moduleMgr->getModule<FullBright>();
	if (fullBrightModule == nullptr)
		fullBrightModule = moduleMgr->getModule<FullBright>();

	static __int64 v7 = 0;
	if (v7 == 0) {
		__int64 v6 = oFunc(a1);
		if (*(bool*)(v6 + 0xF01))
			v7 = *(__int64 *)(v6 + 0x7B8);
		else
			v7 = *(__int64 *)(v6 + 0x128);
	}
	else {
		if (fullBrightModule != nullptr)
			fullBrightModule->gammaPtr = reinterpret_cast<float*>(v7 + 0xF0);
	}

	return oFunc(a1);
}

__int64 __fastcall Hooks::chestScreenController__tick(C_ChestScreenController* a1)
{
	static auto oFunc = g_Hooks.chestScreenController__tickHook->GetOriginal<chestScreenController__tick_t>();

	static ChestStealer* ChestStealerMod = moduleMgr->getModule<ChestStealer>();
	if (ChestStealerMod == nullptr)
		ChestStealerMod = moduleMgr->getModule<ChestStealer>();
	else {
		ChestStealerMod->chestScreenController = a1;
	}
		
	return oFunc(a1);
}

void __fastcall Hooks::clickFunc(__int64 a1, char a2, char a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7, char a8){

	static auto oFunc = g_Hooks.clickHook->GetOriginal<clickFunc_t>();
	static IModule* clickGuiModule = moduleMgr->getModule<ClickGuiMod>();

	if (clickGuiModule == nullptr)
		clickGuiModule = moduleMgr->getModule<ClickGuiMod>();
	else if (clickGuiModule->isEnabled()) {
		if (isTicked) {
			isTicked = false;
			return;
		}
	}
	oFunc(a1,a2,a3,a4,a5,a6,a7,a8); // Call Original Func
}


void __fastcall Hooks::GameMode_tick(C_GameMode * _this)
{
	static auto oTick = g_Hooks.gameMode_tickHook->GetOriginal<GameMode_tick_t>();
	oTick(_this); // Call Original Func

	GameData::updateGameData(_this);
	if (_this->player == g_Data.getLocalPlayer()) {
		moduleMgr->onTick(_this);
	}
}

void __fastcall Hooks::SurvivalMode_tick(C_GameMode * _this)
{
	static auto oTick = g_Hooks.survivalMode_tickHook->GetOriginal<SurvivalMode_tick_t>();
	oTick(_this); // Call Original Func
	GameData::updateGameData(_this);
	if (_this->player == g_Data.getLocalPlayer()) {
		moduleMgr->onTick(_this);
	}
}

__int64 __fastcall Hooks::MoveInputHandler_tick(C_MoveInputHandler* a1, C_Entity* a2)
{
	static auto oTick = g_Hooks.MoveInputHandler_tickHook->GetOriginal<MoveInputHandler_tick_t>();

	static InventoryMove* InventoryMoveMod = moduleMgr->getModule<InventoryMove>();
	if (InventoryMoveMod == nullptr)
		InventoryMoveMod = moduleMgr->getModule<InventoryMove>();
	else{
		InventoryMoveMod->inputHandler = a1;
	}
	static Bhop* bhopMod = moduleMgr->getModule<Bhop>();
	if (bhopMod == nullptr)
		bhopMod = moduleMgr->getModule<Bhop>();
	else {
		bhopMod->inputHandler = a1;
	}
	return oTick(a1, a2);
}

int __fastcall Hooks::BlockLegacy_getRenderLayer(C_BlockLegacy* a1)
{
	static auto oFunc = g_Hooks.BlockLegacy_getRenderLayerHook->GetOriginal<BlockLegacy_getRenderLayer_t>();

	static IModule* XrayModule = moduleMgr->getModule<Xray>();
	if (XrayModule == nullptr)
		XrayModule = moduleMgr->getModule<Xray>();
	else if (XrayModule->isEnabled()) {
		char* text = a1->name.getText();
		if (strstr(text, "ore") == NULL)
			if (strcmp(text, "lava") != NULL)
				if (strcmp(text, "water") != NULL)
					return 9;
	}
	return oFunc(a1);
}



BYTE* __fastcall Hooks::BlockLegacy_getLightEmission(C_BlockLegacy* a1, BYTE* a2)
{
	static auto oFunc = g_Hooks.BlockLegacy_getLightEmissionHook->GetOriginal<BlockLegacy_getLightEmission_t>();

	static IModule* XrayModule = moduleMgr->getModule<Xray>();
	if (XrayModule == nullptr)
		XrayModule = moduleMgr->getModule<Xray>();
	else if (XrayModule->isEnabled()) {
		*a2 = 15;
		return a2;
	}
	return oFunc(a1, a2);
}

__int64 Hooks::LevelRenderer_renderLevel(__int64 _this, __int64 a2, __int64 a3)
{
	static auto oFunc = g_Hooks.LevelRenderer_renderLevelHook->GetOriginal<LevelRenderer_renderLevel_t>();

	using reloadShit_t = void(__fastcall*)(__int64);
	static reloadShit_t reloadShit = reinterpret_cast<reloadShit_t>(Utils::FindSignature("48 8B C4 56 57 41 54 41 56 41 57 48 83 EC ?? 48 C7 40 ?? FE FF FF FF 48 89 58 ?? 48 89 68 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B F9 4C"));

	static IModule* xray = moduleMgr->getModule<Xray>();
	if (xray == nullptr) {
		xray = moduleMgr->getModule<Xray>();
	}
	else {
		static bool lastState = false;
		if (lastState != xray->isEnabled()) {
			lastState = xray->isEnabled();
			unsigned long long *v5; // rdi
			unsigned long long *i; // rbx

			v5 = *(unsigned long long **)(_this + 32);
			for (i = (unsigned long long *)*v5; i != v5; i = (unsigned long long *)*i)
				reloadShit(i[3]);
		}
	}



	return oFunc(_this, a2, a3);
}

void Hooks::HIDController_keyMouse(C_HIDController* _this, void* a2, void* a3)
{
	static auto oFunc = g_Hooks.HIDController_keyMouseHook->GetOriginal<HIDController_keyMouse_t>();
	GameData::setHIDController(_this);
	isTicked = true;
	oFunc(_this, a2, a3); // Call Original Func
	return;
}

void Hooks::GameMode_startDestroyBlock(C_GameMode* _this, vec3_ti* a2, uint8_t face, void* a4, void* a5)
{
	static auto oFunc = g_Hooks.GameMode_startDestroyHook->GetOriginal<GameMode_startDestroyBlock_t>();

	static Nuker* nukerModule = moduleMgr->getModule<Nuker>();
	static IModule* instaBreakModule = moduleMgr->getModule<InstaBreak>();
	if (nukerModule == nullptr || instaBreakModule == nullptr)
	{
		nukerModule = moduleMgr->getModule<Nuker>();
		instaBreakModule = moduleMgr->getModule<InstaBreak>();
	}
	else {
		if (nukerModule->isEnabled()) {
			vec3_ti tempPos;

			const int range = nukerModule->getNukerRadius();
			const bool isVeinMiner = nukerModule->isVeinMiner();

			C_BlockSource* region = g_Data.getLocalPlayer()->region;
			int selectedBlockId = (*(region->getBlock(*a2)->blockLegacy))->blockId;
			uint8_t selectedBlockData = region->getBlock(*a2)->data;

			for (int x = -range; x < range; x++) {
				for (int y = -range; y < range; y++) {
					for (int z = -range; z < range; z++) {
						tempPos.x = a2->x + x;
						tempPos.y = a2->y + y;
						tempPos.z = a2->z + z;
						if (tempPos.y > 0) {
							C_Block* blok = region->getBlock(tempPos);
							uint8_t data = blok->data;
							int id = (*(blok->blockLegacy))->blockId;
							if(id != 0 && (!isVeinMiner || (id == selectedBlockId && data == selectedBlockData)))
								_this->destroyBlock(&tempPos, face);
						}
					}
				}
			}
			return;
		}
		if (instaBreakModule->isEnabled()) {
			_this->destroyBlock(a2, face);
			return;
		}
	}

	oFunc(_this, a2, face, a4, a5);
}

void __fastcall Hooks::MultiLevelPlayer_tick(C_EntityList * _this)
{
	static auto oTick = g_Hooks.MultiLevelPlayerHook->GetOriginal<MultiLevelPlayer_tick_t>();
	oTick(_this); // Call Original Func
	GameData::EntityList_tick(_this);
}

void __fastcall Hooks::ChestBlockActor_tick(C_ChestBlockActor* _this, void* a)
{
	static auto oTick = g_Hooks.ChestBlockActor_tickHook->GetOriginal<ChestBlockActor_tick_t>();
	oTick(_this, a); // Call Original Func
	GameData::addChestToList(_this);
}


void Hooks::Actor_lerpMotion(C_Entity * _this, vec3_t motVec)
{
	static auto oLerp = g_Hooks.Actor_lerpMotionHook->GetOriginal<Actor_lerpMotion_t>();

	if (g_Data.getLocalPlayer() != _this)
		return oLerp(_this, motVec);

	static NoKnockBack* mod = moduleMgr->getModule<NoKnockBack>();
	if (mod == nullptr)
		mod = moduleMgr->getModule<NoKnockBack>();
	else if (mod->isEnabled()) {
		static void* networkSender = reinterpret_cast<void*>(Utils::FindSignature("41 80 BF ?? ?? ?? ?? 00 0F 85 ?? ?? ?? ?? FF"));
		if (networkSender == 0x0)
			logF("Network Sender not Found!!!");
		if (networkSender == _ReturnAddress()) {
			if(mod->xModifier == 0 && mod->yModifier == 0)
				return;
			else {
				motVec.x *= mod->xModifier;
				motVec.y *= mod->yModifier;
				motVec.z *= mod->xModifier;
			}
		}
	}
	oLerp(_this, motVec);
}

signed int Hooks::AppPlatform_getGameEdition(__int64 _this)
{
	static auto oGetEditon = g_Hooks.AppPlatform_getGameEditionHook->GetOriginal<AppPlatform_getGameEdition_t>();

	static EditionFaker* mod = moduleMgr->getModule<EditionFaker>();
	if (mod == nullptr)
		mod = moduleMgr->getModule<EditionFaker>();
	else if (mod->isEnabled()) {
		// Do nothing i guess
		// Do some stuff with modifiers here maybe
		return mod->getFakedEditon(); // Dont call lerpMotion
	}

	return oGetEditon(_this);
}

void Hooks::sendToServer(C_LoopbackPacketSender* a, C_Packet* packet)
{
	static auto oFunc = g_Hooks.sendToServerHook->GetOriginal<sendToServer_tick_t>();

	static IModule* FreecamMod = moduleMgr->getModule<Freecam>();
	static IModule* NoFallMod = moduleMgr->getModule<NoFall>();
	static Blink* BlinkMod = moduleMgr->getModule<Blink>();
	static NoPacket* No_Packet = moduleMgr->getModule<NoPacket>();
	static Criticals* CriticalsMod = moduleMgr->getModule<Criticals>();

	if (FreecamMod == nullptr || NoFallMod == nullptr || BlinkMod == nullptr || No_Packet == nullptr) {
		FreecamMod = moduleMgr->getModule<Freecam>();
		NoFallMod = moduleMgr->getModule<NoFall>();
		BlinkMod = moduleMgr->getModule<Blink>();
		No_Packet = moduleMgr->getModule<NoPacket>();
	}
	else if (No_Packet->isEnabled()) {
		return;
	}
	else if (FreecamMod->isEnabled() || BlinkMod->isEnabled()) {

		C_MovePlayerPacket frenchBoy = C_MovePlayerPacket();
		if (frenchBoy.vTable == packet->vTable)
		{
			if (BlinkMod->isEnabled())
			{
				C_MovePlayerPacket* meme = reinterpret_cast<C_MovePlayerPacket*>(packet);
				meme->onGround = true; //Don't take Fall Damages when turned off
				BlinkMod->PacketMeme.push_back(new C_MovePlayerPacket(*meme)); // Saving the packets
			}
			return; // Dont call sendToServer
		}
	}
	else if (!BlinkMod->isEnabled() && BlinkMod->PacketMeme.size() > 0) {

		for (std::vector<C_MovePlayerPacket*>::iterator it = BlinkMod->PacketMeme.begin(); it != BlinkMod->PacketMeme.end(); ++it)
		{
			oFunc(a, (*it));
			delete *it;
			*it = nullptr;
		}
		BlinkMod->PacketMeme.clear();
		return;
	}
	else if (NoFallMod->isEnabled()) {
		C_MovePlayerPacket frenchBoy = C_MovePlayerPacket();
		C_ActorFallPacket fall = C_ActorFallPacket();
		if (frenchBoy.vTable == packet->vTable) {
			C_MovePlayerPacket* p = reinterpret_cast<C_MovePlayerPacket*>(packet);
			p->onGround = true;
		}
		else if (fall.vTable == packet->vTable)
		{
			C_ActorFallPacket* p = reinterpret_cast<C_ActorFallPacket*>(packet);
			p->fallDistance = 0.f;
		}
	}

	if (CriticalsMod == nullptr)
		CriticalsMod = moduleMgr->getModule<Criticals>();
	else if(CriticalsMod->isEnabled())
	{
		if (g_Data.getClientInstance()->getLocalPlayer() != nullptr) {
			C_MovePlayerPacket movePacket = C_MovePlayerPacket();
			if (movePacket.vTable == packet->vTable) {
				C_MovePlayerPacket* p = reinterpret_cast<C_MovePlayerPacket*>(packet);
				if (g_Data.getLocalPlayer()->onGround)
					p->onGround = false;
			}
		}
	}

#ifdef TEST_DEBUG
	C_InventoryTransactionPacket Packet0 = C_InventoryTransactionPacket();
	if (packet->vTable == Packet0.vTable)
	{
		C_InventoryTransactionPacket* y = reinterpret_cast<C_InventoryTransactionPacket*>(packet);
		logF("action type : %d", y->complexTransaction->actionType);
	}
#endif
	oFunc(a, packet);
}

float Hooks::LevelRendererPlayer_getFov(__int64 _this, float a2, bool a3)
{
	static auto oGetFov = g_Hooks.levelRendererPlayer_getFovHook->GetOriginal<getFov_t>();
	static void* renderItemInHand = reinterpret_cast<void*>(Utils::FindSignature("F3 44 0F 10 2D ?? ?? ?? ?? F3 41 0F 59 C5 0F 28 DE F3"));
	static void* setupCamera = reinterpret_cast<void*>(Utils::FindSignature("48 8B 8B ?? ?? ?? ?? 0F 28 F8"));

	if (_ReturnAddress() == renderItemInHand) {
		//static float yess = 0;
		//yess += 0.03f;
		//return 60 + 40 + sinf(yess) * 50;
		return oGetFov(_this, a2, a3);
	}
	if (_ReturnAddress() == setupCamera) {
		return oGetFov(_this, a2, a3);
	}
#ifdef _DEBUG
	logF("LevelRendererPlayer_getFov Return Addres: %llX", _ReturnAddress());
	__debugbreak(); // IF we reach here, a sig is broken
#endif
	return oGetFov(_this, a2, a3);
}

bool Hooks::Mob_isAlive(C_Entity* _this)
{
	static auto oIsAlive = g_Hooks.mob_isAliveHook->GetOriginal<mob_isAlive_T>();

	//if (a1 == g_Data.getLocalPlayer())
		//return true;

	return oIsAlive(_this);
}

void Hooks::pleaseAutoComplete(__int64 a1, __int64 a2, TextHolder * text, int a4)
{
	static auto oAutoComplete = g_Hooks.autoComplete_Hook->GetOriginal<autoComplete_t>();
	char* tx = text->getText();
	if (tx != nullptr && text->getTextLength() >= 1 && tx[0] == '.') {
		std::string search = tx + 1; // Dont include the '.'
		std::transform(search.begin(), search.end(), search.begin(), ::tolower); // make the search text lowercase

		struct LilPlump {
			std::string cmdAlias;
			IMCCommand* command = 0;
			bool shouldReplace = true;

			bool operator<(const LilPlump &o) const {
				return cmdAlias < o.cmdAlias;
			}
		}; // This is needed so the std::set sorts it alphabetically

		std::set<LilPlump> searchResults;

		std::vector<IMCCommand*>* commandList = cmdMgr->getCommandList();
		for (std::vector<IMCCommand*>::iterator it = commandList->begin(); it != commandList->end(); ++it) { // Loop through commands
			IMCCommand* c = *it;
			auto* aliasList = c->getAliasList();
			for (std::vector<std::string>::iterator it = aliasList->begin(); it != aliasList->end(); ++it) { // Loop through aliases
				std::string cmd = *it;
				LilPlump plump;

				for (size_t i = 0; i < search.size(); i++) { // Loop through search string
					char car = search.at(i);
					if (car == ' ' && i == cmd.size()) {
						plump.shouldReplace = false;
						break;
					}
					else if (i >= cmd.size())
						goto nope;

					if (car != cmd.at(i)) // and compare
						goto nope;
				}
				// Not at nope? Then we got a good result!
				{
					cmd.insert(0, 1, '.'); // Prepend the '.'

					plump.cmdAlias = cmd;
					plump.command = c;
					searchResults.emplace(plump);
				}

			nope:
				continue;
			}
		}

		if (searchResults.size() > 0) {
			LilPlump firstResult = (*searchResults.begin());

			if (searchResults.size() > 1) {
				g_Data.getGuiData()->displayClientMessageF("==========");
				for (auto it = searchResults.begin(); it != searchResults.end(); ++it) {
					LilPlump plump = *it;
					g_Data.getGuiData()->displayClientMessageF("%s%s - %s%s", plump.cmdAlias.c_str(), GRAY, ITALIC, plump.command->getDescription());
				}
			}
			else {
				g_Data.getGuiData()->displayClientMessageF("==========");
				if (firstResult.command->getUsage()[0] == 0x0)
					g_Data.getGuiData()->displayClientMessageF("%s%s %s- %s", WHITE, firstResult.cmdAlias.c_str(), GRAY, firstResult.command->getDescription());
				else
					g_Data.getGuiData()->displayClientMessageF("%s%s %s %s- %s", WHITE, firstResult.cmdAlias.c_str(), firstResult.command->getUsage(), GRAY, firstResult.command->getDescription());
			}

			if (firstResult.shouldReplace) {
				if (search.size() == firstResult.cmdAlias.size() - 1 && searchResults.size() == 1)
					firstResult.cmdAlias.append(" ");
				text->setText(firstResult.cmdAlias); // Set text
				// now sync with the UI thread that shows the cursor n stuff
				// If we loose this sig we are kinda fucked
				using syncShit = void(__fastcall*)(TextHolder*, TextHolder*);
				static syncShit sync = reinterpret_cast<syncShit>(0);
				if (sync == 0) {
					uintptr_t sigOffset = Utils::FindSignature("E8 ?? ?? ?? ?? 48 8D 8B ?? ?? ?? ?? 0F 57 C0");
					if (sigOffset != 0x0) {
						int offset = *reinterpret_cast<int*>((sigOffset + 1)); // Get Offset from code
						sync = reinterpret_cast<syncShit>(sigOffset + offset + /*length of instruction*/ 5); // Offset is relative
					}
				}else
					sync(text, text);
			}

		}

		return;
	}
	oAutoComplete(a1, a2, text, a4);

}

void __fastcall Hooks::ChatScreenController_sendChatMessage(uint8_t * _this)
{
	static auto oSendMessage = g_Hooks.chatScreen_sendMessageHook->GetOriginal<ChatScreen_sendChatMessage_t>();

	using dequeuePushback_t = void(__fastcall*)(__int64*, __int64);
	static dequeuePushback_t dequeuePushBack = reinterpret_cast<dequeuePushback_t>(Utils::FindSignature("48 89 5C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 48 8B D9 48 8B F2 48 8B 49 ?? 48 8B 43 ?? 48 FF C0 48 3B C8 77 ?? 48 8B CB E8 ?? ?? ?? ?? 48 8B 4B ?? 48 8D 41 ?? 48 21 43 ?? 48 8B 53 ?? 48 03 53 ?? 48 8B 43 ?? 48 8B 4B ?? 48 FF C8 48 23 D0 48 83 3C D1 00 48 8D 3C D5 00 00 00 00 75 ?? B9 20"));

	using sub_140074FA0_t = void(__fastcall*)(__int64);
	static sub_140074FA0_t sub_140074FA0 = reinterpret_cast<sub_140074FA0_t>(Utils::FindSignature("40 53 48 83 EC ?? 48 8B 51 ?? 48 8B D9 48 83 FA 10 72 ?? 48 8B 09 48 FF C2 48 81 FA 00 10 00 00 72 ?? 4C 8B 41 ?? 48 83 C2 ?? 49 2B C8 48 8D 41 ?? 48 83 F8 ?? 77 ?? 49 8B C8 E8 ?? ?? ?? ?? 48 C7 43 ?? 00 00 00 00"));

	uintptr_t* textLength = reinterpret_cast<uintptr_t*>(_this + 0x6C0);
	if (*textLength) {
		char* message = reinterpret_cast<char*>(_this + 0x6B0);
		if (*reinterpret_cast<__int64*>(_this + 0x6C8) >= 0x10)
			message = *reinterpret_cast<char**>(message);

		if (*message == '.') {
			cmdMgr->execute(message);

			__int64* i = 0;
			for (i = *reinterpret_cast<__int64**>(_this + 0x6D8); i[4] > 0x64ui64; i = *reinterpret_cast<__int64**>(_this + 0x6D8))
			{
				sub_140074FA0(*reinterpret_cast<__int64*>(i[1] + 8 * (i[3] & (i[2] - 1i64))));
				bool v15 = i[4]-- == 1i64;
				if (v15)
					i[3] = 0i64;
				else
					++i[3];
			}

			dequeuePushBack(i, reinterpret_cast<__int64>(_this + 0x6B0)); // This will put the command in the chat history (Arrow up/down)
			*reinterpret_cast<__int64*>(_this + 0x6E0) = *reinterpret_cast<__int64*>(*reinterpret_cast<__int64*>(_this + 0x6D8) + 0x20);

			*reinterpret_cast<__int64*>(_this + 0x6C0) = 0i64;
			*message = 0x0; // Remove command in textbox
			*textLength = 0x0; // text length
			return;
		}
	}
	oSendMessage(_this);
}

HRESULT __stdcall Hooks::d3d11_present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static auto oPresent = g_Hooks.d3d11_presentHook->GetOriginal<d3d11_present_t>();
	return oPresent(pSwapChain, SyncInterval, Flags);
}

__int64 __fastcall Hooks::setupAndRender(C_UIScene* uiscene, __int64 screencontext)
{
	static auto oSetup = g_Hooks.setupRenderHook->GetOriginal<setupRender_t>();
	
	g_Hooks.shouldRender = uiscene->isPlayScreen();
	logF("setupAndRender");
	
	return oSetup(uiscene, screencontext);
}

__int64 __fastcall Hooks::uiscene_render(C_UIScene * uiscene, __int64 screencontext)
{
	static auto oRender = g_Hooks.uiscene_RenderHook->GetOriginal<uirender_t>();

	g_Hooks.shouldRender = uiscene->isPlayScreen();
	if (!g_Hooks.shouldRender) {
		TextHolder* alloc = new TextHolder();

		uiscene->getScreenName(alloc);

		if (strcmp(alloc->getText(), "hud_screen") == 0 || strcmp(alloc->getText(), "start_screen") == 0 || (alloc->getTextLength() >= 11 && strncmp(alloc->getText(), "play_screen", 11)) == 0)
			g_Hooks.shouldRender = true;
		
		delete alloc;
	}

	return oRender(uiscene, screencontext);
}

__int64 __fastcall Hooks::renderText(__int64 a1, C_MinecraftUIRenderContext* renderCtx)
{
	static auto oText = g_Hooks.renderTextHook->GetOriginal<renderText_t>();
	C_GuiData* dat = g_Data.getClientInstance()->getGuiData(); 
	DrawUtils::setCtx(renderCtx, dat);
	if(GameData::shouldHide())
		return  oText(a1, renderCtx);

	// Call PreRender() functions
	moduleMgr->onPreRender();
	DrawUtils::flush();

	__int64 retval = oText(a1, renderCtx);

#ifdef PERFORMANCE_TEST
	std::chrono::steady_clock::time_point beginPostRender = std::chrono::steady_clock::now();
#endif

	bool shouldRenderArrayList = true;
	// Call PostRender() functions
	{
		moduleMgr->onPostRender();
		static IModule* TabGuiModule = moduleMgr->getModule<TabGuiMod>();
		if (TabGuiModule == nullptr)
			TabGuiModule = moduleMgr->getModule<TabGuiMod>();
		else if (TabGuiModule->isEnabled()) {
			TabGui::render();
		}
		
		static IModule* ClickGuiModule = moduleMgr->getModule<ClickGuiMod>();
		if (ClickGuiModule == nullptr)
			ClickGuiModule = moduleMgr->getModule<ClickGuiMod>();
		else if (ClickGuiModule->isEnabled()) {
			ClickGui::render();
			shouldRenderArrayList = false;
		}
	}

	{
		// Display ArrayList on the Right?
		static constexpr bool isOnRightSide = true;
		static float rcolors[4]; // Rainbow color array RGBA
		static float disabledRcolors[4]; // Rainbow Colors, but for disabled modules
		static std::string horionStr = std::string("Horion");					 // Static Horion logo / text
		static float       horionStrWidth = DrawUtils::getTextWidth(&horionStr); // Graphical Width of Horion logo / text
		static std::string dlStr = std::string("discord.gg/8CRYQWM");
		static float       dlStrWidth = DrawUtils::getTextWidth(&horionStr);

		float yOffset = 0; // Offset of next Text
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;

		vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();
		mousePos.div(windowSizeReal);
		mousePos.mul(windowSize);

		// Rainbow color updates
		{
			DrawUtils::rainbow(rcolors); // Increase Hue of rainbow color array
			disabledRcolors[0] = min(1, rcolors[0] * 0.4f + 0.2f);
			disabledRcolors[1] = min(1, rcolors[1] * 0.4f + 0.2f);
			disabledRcolors[2] = min(1, rcolors[2] * 0.4f + 0.2f);
			disabledRcolors[3] = 1;
		}

		// Draw Horion logo
		{
			DrawUtils::drawText(vec2_t(windowSize.x - horionStrWidth - 12.0f,windowSize.y - 20), &horionStr, nullptr, 1.5f);
			DrawUtils::drawText(vec2_t(windowSize.x - dlStrWidth - 42.75f,windowSize.y - 8.75f), &dlStr, nullptr, 0.85f);
		}

		// Draw ArrayList
		if (moduleMgr->isInitialized() && shouldRenderArrayList) {
			struct IModuleContainer {
				// Struct used to Sort IModules in a std::set
				IModule* backingModule;
				std::string moduleName;
				bool enabled;
				int keybind;
				float textWidth;

				IModuleContainer(IModule* mod) {
					const char* moduleNameChr = mod->getModuleName();
					this->enabled = mod->isEnabled();
					this->keybind = mod->getKeybind();
					this->backingModule = mod;

					if (keybind == 0x0)
						moduleName = moduleNameChr;
					else {
						char yikes[50];
						sprintf_s(yikes, 50, "%s [%s]", moduleNameChr, Utils::getKeybindName(keybind));
						moduleName = yikes;
					}

					this->textWidth = DrawUtils::getTextWidth(&moduleName);
				}

				bool operator<(const IModuleContainer &other) const {

					if (enabled) {
						if (!other.enabled) // We are enabled
							return true;
					}
					else if (other.enabled) // They are enabled
						return false;

					if (this->textWidth == other.textWidth)
						return moduleName < other.moduleName;
					return this->textWidth > other.textWidth;
				}
			};

			// Parameters
			static constexpr float textPadding = 1.0f;
			static constexpr float textSize = 1.0f;
			static constexpr float textHeight = textSize * 10.0f;

			// Mouse click detector
			static bool wasLeftMouseDown = GameData::isLeftClickDown(); // Last isDown value
			bool leftMouseDown = GameData::isLeftClickDown(); // current isDown value

			bool executeClick = leftMouseDown && leftMouseDown != wasLeftMouseDown; // isDown == true AND (current state IS NOT last state)
			wasLeftMouseDown = leftMouseDown; // Set last isDown value

			// Show disabled Modules?
			//const bool extendedArraylist = g_Data.getLocalPlayer() == nullptr ? /* not ingame */ true : /* ingame */(GameData::canUseMoveKeys() ? false : true);
			constexpr bool extendedArraylist = false;
			std::set<IModuleContainer> modContainerList;
			// Fill modContainerList with Modules
			{
				std::vector<IModule*>* moduleList = moduleMgr->getModuleList();

				for (std::vector<IModule*>::iterator it = moduleList->begin(); it != moduleList->end(); ++it) {
					if (extendedArraylist || (*it)->isEnabled())
						modContainerList.emplace(IModuleContainer(*it));
				}
			}

			// Loop through mods to display Labels
			for (std::set<IModuleContainer>::iterator it = modContainerList.begin(); it != modContainerList.end(); ++it) {

				if (!extendedArraylist && !it->enabled)
					continue;

				std::string textStr = it->moduleName;
				float textWidth = it->textWidth;

				float xOffset = isOnRightSide ? windowSize.x - textWidth - (textPadding * 2) : 0;
				vec2_t textPos = vec2_t(
					xOffset + textPadding,
					yOffset + textPadding
				);
				vec4_t rectPos = vec4_t(
					xOffset,
					yOffset,
					isOnRightSide ? windowSize.x : textWidth + (textPadding * 2),
					yOffset + textPadding * 2 + textHeight
				);

				DrawUtils::drawText(textPos, &textStr, new MC_Color(it->enabled ? rcolors : disabledRcolors), textSize);
				if (!GameData::canUseMoveKeys() && rectPos.contains(&mousePos)) {

					if (leftMouseDown) {
						DrawUtils::fillRectangle(rectPos, MC_Color(0.4f, 0.9f, 0.4f, 0.1f), it->enabled ? 0.6f : 0.6f);
						if (executeClick)
							it->backingModule->toggle();
					}
					else
						DrawUtils::fillRectangle(rectPos, MC_Color(0.3f, 0.7f, 0.3f, 0.1f), it->enabled ? 0.4f : 0.15f);
				}
				/*else
					DrawUtils::fillRectangle(rectPos, MC_Color(0.f, 0.1f, 0.1f, 0.1f), it->enabled ? 0.4f : 0.15f);*/

				yOffset += textHeight + (textPadding * 2);
			}
			modContainerList.clear();
		}
	}
	

	DrawUtils::flush();

#ifdef PERFORMANCE_TEST
	std::chrono::steady_clock::time_point endRender = std::chrono::steady_clock::now();
	//logF("PreRender: %.1f", std::chrono::duration_cast<std::chrono::microseconds>(endPreRender - beginPreRender).count() / 1000.f);
	logF("Render: %.2fms", std::chrono::duration_cast<std::chrono::microseconds>(endRender - beginPostRender).count() / 1000.f);
#endif
	
	return retval;
}

char* __fastcall Hooks::I8n_get(void* f, char* str)
{
	static auto oGet = g_Hooks.renderTextHook->GetOriginal<I8n_get_t>();

	//if (strcmp(str, "menu.play") == 0)
		//return &yote;
	return oGet(f, str);
}

float * Hooks::Dimension_getFogColor(__int64 _this, float * color, float brightness)
{
	static auto oGetFogColor = g_Hooks.Dimension_getFogColorHook->GetOriginal<Dimension_getFogColor_t>();

	static float rcolors[4];

	static IModule* mod = moduleMgr->getModule<RainbowSky>();
	if (mod == nullptr)
		mod = moduleMgr->getModule<RainbowSky>();
	else if (mod->isEnabled()) {
		if (rcolors[3] < 1) {
			rcolors[0] = 1;
			rcolors[1] = 0.2f;
			rcolors[2] = 0.2f;
			rcolors[3] = 1;
		}

		Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]); // perfect code, dont question this

		rcolors[0] += 0.001f;
		if (rcolors[0] >= 1)
			rcolors[0] = 0;

		Utils::ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);

		return rcolors;
	}
	return oGetFogColor(_this, color, brightness);
}
