#include "Hooks.h"

#include "../SDK/Tag.h"

#include <algorithm>

#include <glm/mat4x4.hpp>         // mat4
#include <glm/trigonometric.hpp>  //radians

#include <glm/ext/matrix_transform.hpp> // perspective, translate, rotate
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/constants.hpp>

Hooks g_Hooks;
bool isTicked = false;
bool overrideStyledReturn = false;
TextHolder styledReturnText;
//#define TEST_DEBUG

void Hooks::Init() {
	logF("Setting up Hooks...");
	// clang-format off
	// Vtables
	{
		// GameMode::vtable
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? C7 41 18 FF FF FF FF");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** gameModeVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (gameModeVtable == 0x0 || sigOffset == 0x0)
				logF("C_GameMode signature not working!!!");
			else {
				g_Hooks.GameMode_startDestroyBlockHook = std::make_unique<FuncHook>(gameModeVtable[1], Hooks::GameMode_startDestroyBlock);

				g_Hooks.GameMode_tickHook = std::make_unique<FuncHook>(gameModeVtable[9], Hooks::GameMode_tick);
				
				g_Hooks.GameMode_getPickRangeHook = std::make_unique<FuncHook>(gameModeVtable[10], Hooks::GameMode_getPickRange);
				
				g_Hooks.GameMode_attackHook = std::make_unique<FuncHook>(gameModeVtable[14], Hooks::GameMode_attack);
			}
		}

		// BlockLegacy::vtable
		{
			intptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 01 4C 8B 72 ? 48 B9");  // BlockLegacy constructor
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** blockLegacyVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
			if (blockLegacyVtable == 0x0 || sigOffset == 0x0)
				logF("C_BlockLegacy signature not working!!!");
			else {
				g_Hooks.BlockLegacy_getRenderLayerHook = std::make_unique<FuncHook>(blockLegacyVtable[168], Hooks::BlockLegacy_getRenderLayer);
				g_Hooks.BlockLegacy_getLightEmissionHook = std::make_unique<FuncHook>(blockLegacyVtable[171], Hooks::BlockLegacy_getLightEmission);
			}
		}

		// LocalPlayer::vtable
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 07 48 8D 8F ?? ?? ?? ?? 48 8B 87");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** localPlayerVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (localPlayerVtable == 0x0 || sigOffset == 0x0)
				logF("C_LocalPlayer signature not working!!!");
			else {
				g_Hooks.Actor_isInWaterHook = std::make_unique<FuncHook>(localPlayerVtable[65], Hooks::Actor_isInWater);

				g_Hooks.Actor_startSwimmingHook = std::make_unique<FuncHook>(localPlayerVtable[182], Hooks::Actor_startSwimming);

				g_Hooks.Actor_ascendLadderHook = std::make_unique<FuncHook>(localPlayerVtable[333], Hooks::Actor_ascendLadder);
			}
		}

		// MoveInputHandler::vtable
		{
			uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 49 89 48 ? 49 89 80 ? ? ? ? 49 89 80 ? ? ? ? 48 39 87 ? ? ? ? 74 20 48 8B 8F");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** moveInputVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
			if (moveInputVtable == 0x0 || sigOffset == 0x0)
				logF("C_GameMode signature not working!!!");
			else {
				g_Hooks.MoveInputHandler_tickHook = std::make_unique<FuncHook>(moveInputVtable[1], Hooks::MoveInputHandler_tick);
			}
		}

		// PackAccessStrategy vtables for isTrusted
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 49 8D 76 50");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** directoryPackVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset +  7);
			
			{
				g_Hooks.DirectoryPackAccessStrategy__isTrustedHook = std::make_unique<FuncHook>(directoryPackVtable[6], Hooks::DirectoryPackAccessStrategy__isTrusted);
			}

			uintptr_t sigOffset2 = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 03 49 8D 57");
			int offset2 = *reinterpret_cast<int*>(sigOffset2 + 3);
			uintptr_t** directoryPackVtable2 = reinterpret_cast<uintptr_t**>(sigOffset2 + offset2 +  7);
			
			{
				g_Hooks.ZipPackAccessStrategy__isTrustedHook = std::make_unique<FuncHook>(directoryPackVtable2[6], Hooks::ReturnTrue);
			}
			g_Hooks.SkinRepository___checkSignatureFileInPack = std::make_unique<FuncHook>(FindSignature("48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B 39 48 8B 59 ? 48 85 DB"), Hooks::ReturnTrue);			
		}
	}

	// d3d11
	/*{
		/*const auto hModDXGI = GetModuleHandle(L"DXGI.dll");
		const auto hModD3D11 = GetModuleHandle(L"D3D11.dll");

		const auto hD3D11CreateDeviceAndSwapChain = static_cast<LPVOID>(GetProcAddress(hModD3D11, "D3D11CreateDeviceAndSwapChain"));

		const D3D_FEATURE_LEVEL featureLevelArray[3] = {
			D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0
		};


		DXGI_SWAP_CHAIN_DESC sd;
		{
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = 2;
			sd.BufferDesc.Width = 0;
			sd.BufferDesc.Height = 0;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = (HWND)0x780f84; // insert window found from process hacker here
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = TRUE;
			sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		}
	
        IDXGISwapChain* pSwapChain;

		const auto hr11 = static_cast<HRESULT(WINAPI *)(
        IDXGIAdapter*,
        D3D_DRIVER_TYPE,
        HMODULE,
        UINT,
        const D3D_FEATURE_LEVEL*,
        UINT,
        UINT,
        const DXGI_SWAP_CHAIN_DESC*,
        IDXGISwapChain**,
        ID3D11Device**,
        D3D_FEATURE_LEVEL*,
        ID3D11DeviceContext**)>(hD3D11CreateDeviceAndSwapChain)(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			featureLevelArray,
			1,
			D3D11_SDK_VERSION,
			&sd,
			&pSwapChain,
			0,
			0,
			0);

		if (pSwapChain) {
			logF("swap vtable: %llX", *pSwapChain);
			pSwapChain->Release();
		}*
		
		uintptr_t sigOffset = FindSignature("48 8B 0D ?? ?? ?? ?? 48 8B 91 ?? ?? ?? ?? E8");
		if (sigOffset != 0x0) {
			int startOffsetOffset = *reinterpret_cast<int*>((sigOffset + 3));
			uintptr_t startOffset = sigOffset + startOffsetOffset + /*length of instruction/ 7;  
			size_t secondOffset = (size_t) *reinterpret_cast<int*>((sigOffset + 10));
			auto swapChain = g_Data.getSlimMem()->ReadPtr<__int64>(startOffset, {0, secondOffset, 0x170});
			auto vtable = *reinterpret_cast<uintptr_t**>(swapChain);

			//logF(" %llX",vtable[8]);

			g_Hooks.swapchain__presentHook = std::make_unique<FuncHook>(vtable[8], Hooks::swapChain__present);
			g_Hooks.swapchain__resizeBuffersHook = std::make_unique<FuncHook>(vtable[13], Hooks::swapChain__ResizeBuffers);
		}
	}*/

	// Signatures
	{
		void* surv_tick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 48 89 74 24 18 55 57 41 56 48 8D 6C 24 ?? 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 48 8B F9 F3"));
		g_Hooks.SurvivalMode_tickHook = std::make_unique<FuncHook>(surv_tick, Hooks::SurvivalMode_tick);

		void* _sendChatMessage = reinterpret_cast<void*>(FindSignature("48 89 5C 24 08 57 48 83 EC ?? 48 8B D9 48 83 B9"));
		g_Hooks.ChatScreenController_sendChatMessageHook = std::make_unique<FuncHook>(_sendChatMessage, Hooks::ChatScreenController_sendChatMessage);

		void* _renderText = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 0F 29 70 ?? 0F 29 78 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 4C 8B F2 48 89 54 24"));
		g_Hooks.RenderTextHook = std::make_unique<FuncHook>(_renderText, Hooks::RenderText);
		g_Hooks.RenderTextHook->enableHook();

		void* setupRender = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B DA 48 8B F9 33 D2 41 B8"));
		g_Hooks.UIScene_setupAndRenderHook = std::make_unique<FuncHook>(setupRender, Hooks::UIScene_setupAndRender);

		void* render = reinterpret_cast<void*>(FindSignature("48 89 5C 24 18 56 57 41 56 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B FA 48 8B D9 41"));
		g_Hooks.UIScene_renderHook = std::make_unique<FuncHook>(render, Hooks::UIScene_render);

		void* fogColorFunc = reinterpret_cast<void*>(FindSignature("41 0F 10 08 48 8B C2 0F"));
		g_Hooks.Dimension_getFogColorHook = std::make_unique<FuncHook>(fogColorFunc, Hooks::Dimension_getFogColor);

		void* timeOfDay = reinterpret_cast<void*>(FindSignature("44 8B C2 B8 F1 19 76 05"));
		g_Hooks.Dimension_getTimeOfDayHook = std::make_unique<FuncHook>(timeOfDay, Hooks::Dimension_getTimeOfDay);

		void* chestTick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 18 56 57 41 56 48 83 EC ?? 48 83 79"));
		g_Hooks.ChestBlockActor_tickHook = std::make_unique<FuncHook>(chestTick, Hooks::ChestBlockActor_tick);

		void* lerpFunc = reinterpret_cast<void*>(FindSignature("8B 02 89 81 ?? 04 ?? ?? 8B 42 04 89 81 ?? ?? ?? ?? 8B 42 08 89 81 ?? ?? ?? ?? C3"));
		g_Hooks.Actor_lerpMotionHook = std::make_unique<FuncHook>(lerpFunc, Hooks::Actor_lerpMotion);

		void* getGameEdition = reinterpret_cast<void*>(FindSignature("8B 91 ?? ?? ?? ?? 85 D2 74 1C 83 EA 01"));
		g_Hooks.AppPlatform_getGameEditionHook = std::make_unique<FuncHook>(getGameEdition, Hooks::AppPlatform_getGameEdition);

		void* autoComplete = reinterpret_cast<void*>(FindSignature("48 8B C4 55 57 41 56 48 8D 68 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 ?? 48 89 70 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 41 8B F9"));
		g_Hooks.PleaseAutoCompleteHook = std::make_unique<FuncHook>(autoComplete, Hooks::PleaseAutoComplete);

		uintptr_t** packetSenderVtable = reinterpret_cast<uintptr_t**>(*(uintptr_t*)g_Data.getClientInstance()->loopbackPacketSender);
		g_Hooks.LoopbackPacketSender_sendToServerHook = std::make_unique<FuncHook>(packetSenderVtable[2], Hooks::LoopbackPacketSender_sendToServer);

		void* getFov = reinterpret_cast<void*>(FindSignature("40 53 48 83 EC 70 0F 29 7C 24 ? 44 0F 29 4C 24 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? F3 0F 10 3D ? ? ? ? 44 0F"));
		g_Hooks.LevelRendererPlayer_getFovHook = std::make_unique<FuncHook>(getFov, Hooks::LevelRendererPlayer_getFov);

		void* tick_entityList = reinterpret_cast<void*>(FindSignature("48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? E8 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B D8 ?? ?? ?? ?? ?? ?? 48 99"));
		g_Hooks.MultiLevelPlayer_tickHook = std::make_unique<FuncHook>(tick_entityList, Hooks::MultiLevelPlayer_tick);

		void* keyMouseFunc = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC ? 0F 29 74 24 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 E0 49 8B F0 48 8B F9"));
		g_Hooks.HIDController_keyMouseHook = std::make_unique<FuncHook>(keyMouseFunc, Hooks::HIDController_keyMouse);

		void* renderLevel = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 48 89 74 24 20 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 49 8B F8 48 8B DA"));
		g_Hooks.LevelRenderer_renderLevelHook = std::make_unique<FuncHook>(renderLevel, Hooks::LevelRenderer_renderLevel);

		void* clickHook = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24 ? ? ? ? 48 8B D9"));
		g_Hooks.ClickFuncHook = std::make_unique<FuncHook>(clickHook, Hooks::ClickFunc);

		void* chestScreenControllerTick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 08 57 48 83 EC 20 48 8B F9 E8 ?? ?? ?? ?? 48 8B 17 48 8B CF 8B D8 FF 92 ?? ?? ?? ?? 84 C0 74 31"));
		g_Hooks.ChestScreenController_tickHook = std::make_unique<FuncHook>(chestScreenControllerTick, Hooks::ChestScreenController_tick);

		void* fullbright = reinterpret_cast<void*>(FindSignature("48 83 EC 28 80 B9 ?? ?? ?? ?? 00 48 8D 54 24 30 74 36 41 B8 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 10 48 85 D2 74 3C 48 8B 8A ?? ?? ?? ?? 48 85 C9 74 0A E8 ?? ?? ?? ?? 48 83 C4 28 C3"));
		g_Hooks.GetGammaHook = std::make_unique<FuncHook>(fullbright, Hooks::GetGamma);

		// Mob::_jumpFromGround
		void* jump = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 57 48 83 EC ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B 19 48 8D"));
		g_Hooks.JumpPowerHook = std::make_unique<FuncHook>(jump, Hooks::JumpPower);

		void* onAppSuspended = reinterpret_cast<void*>(FindSignature("48 8B C4 55 48 8B EC 48 83 EC ? 48 C7 45 ? ? ? ? ? 48 89 58 ? 48 89 70 ? 48 89 78 ? 48 8B F1 E8 ? ? ? ? 48 8B D8 48 8B C8"));
		g_Hooks.MinecraftGame_onAppSuspendedHook = std::make_unique<FuncHook>(onAppSuspended, Hooks::MinecraftGame_onAppSuspended);

		void* RakNetInstance__tick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 48 89 74 24 18 55 57 41 54 41 56 41 57 48 8D ?? 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 48 8B F9 45 33 E4 4C"));
		g_Hooks.RakNetInstance_tickHook = std::make_unique<FuncHook>(RakNetInstance__tick, Hooks::RakNetInstance_tick);

		void* ConnectionRequest__create = reinterpret_cast<void*>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 49 8B D9 4D 8B F8"));
		g_Hooks.ConnectionRequest_createHook = std::make_unique<FuncHook>(ConnectionRequest__create, Hooks::ConnectionRequest_create);

		void* PaintingRenderer__renderAddr = reinterpret_cast<void*>(FindSignature("48 8B C4 57 41 54 41 55 41 56 41 57 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 58 ?? 48 89 68 ?? 48 89 70 ?? 4D 8B F0 4C 8B FA 48 8B F1 B9 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ??"));
		g_Hooks.PaintingRenderer__renderHook = std::make_unique<FuncHook>(PaintingRenderer__renderAddr, Hooks::PaintingRenderer__render);

		void* _getSkinPack = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B E2 48 8B F1"));
		g_Hooks.SkinRepository___loadSkinPackHook = std::make_unique<FuncHook>(_getSkinPack, Hooks::SkinRepository___loadSkinPack);
		
		void* _toStyledString = reinterpret_cast<void*>(FindSignature("40 55 56 57 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? FE FF FF FF 48 89 9C 24 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B FA 48 8B D9 48 89 54 24 ?? 33 D2"));
		g_Hooks.toStyledStringHook = std::make_unique<FuncHook>(_toStyledString, Hooks::toStyledString);
		 
		void* InGamePlayScreen___renderLevel = reinterpret_cast<void*>(FindSignature("48 89 5C 24 20 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 0F 29 B4 24 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 49 8B D8 4C"));
		g_Hooks.InGamePlayScreen___renderLevelHook = std::make_unique<FuncHook>(InGamePlayScreen___renderLevel, Hooks::InGamePlayScreen___renderLevel);

#ifdef TEST_DEBUG
		void* addAction = reinterpret_cast<void*>(FindSignature("40 56 57 41 54 41 56 41 57 48 83 EC 30 48 C7 44 24 ? ? ? ? ? 48 89 5C 24 ? 48 89 6C 24 ? 45 0F B6 F8 4C 8B F2 48 8B F9 48 8B 01 48 8B 88 ? ? ? ?"));
		g_Hooks.InventoryTransactionManager__addActionHook = std::make_unique<FuncHook>(addAction, Hooks::InventoryTransactionManager__addAction);
#endif
		
		void* localPlayerUpdateFromCam = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ?? 57 48 83 EC ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 80 BA"));
		g_Hooks.LocalPlayer__updateFromCameraHook = std::make_unique<FuncHook>(localPlayerUpdateFromCam, Hooks::LocalPlayer__updateFromCamera);

		void* MobIsImmobile = reinterpret_cast<void*>(FindSignature("40 53 48 83 EC ? 80 B9 ? ? ? ? ? 48 8B D9 75 5E"));
		g_Hooks.Mob__isImmobileHook = std::make_unique<FuncHook>(MobIsImmobile, Hooks::Mob__isImmobile);

		void* renderNameTags = reinterpret_cast<void*>(FindSignature("48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 C7 45 ? ? ? ? ? 48 89 58 ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 48 8B 05 ? ? ? ? 48 33 C4"));
		g_Hooks.LevelRendererPlayer__renderNameTagsHook = std::make_unique<FuncHook>(renderNameTags, Hooks::LevelRendererPlayer__renderNameTags);
	
		static constexpr auto counterStart = __COUNTER__ + 1;
		#define lambda_counter (__COUNTER__ - counterStart)
		
		void* levelRendererBobView = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ?? 57 48 81 EC ?? ?? ?? ?? 48 8B D9 0F 29 B4 24 ?? ?? ?? ?? 48 8B 89"));

		static auto bobViewHookF = [](__int64 _this, glm::mat4& matrix, float lerpT){
			static auto origFunc = g_Hooks.lambdaHooks.at(lambda_counter)->GetFastcall<void, __int64, glm::mat4&, float>();
			
			/*auto p = g_Data.getLocalPlayer();
			float degrees = fmodf(p->getPosOld()->lerp(p->getPos(), lerpT).x, 5) - 2.5f;
			degrees *= 180 / 2.5f;

			auto pos = g_Data.getClientInstance()->levelRenderer->origin;
			
			glm::mat4 View = matrix;
			
			matrix = View;
			//matrix = glm::rotate<float>(matrix, glm::radians<float>(degrees), glm::vec3(0, 0, 1));*/
			return origFunc(_this, matrix, lerpT);
		};
		
		std::shared_ptr<FuncHook> bobViewHook = std::make_shared<FuncHook>(levelRendererBobView, (decltype(&bobViewHookF.operator()))bobViewHookF);

		g_Hooks.lambdaHooks.push_back(bobViewHook);

		#undef lambda_counter

		logF("Hooks initialized");
	}

// clang-format on
}

void Hooks::Restore() {
	MH_DisableHook(MH_ALL_HOOKS);
	Sleep(10);
}

void Hooks::Enable() {
	logF("Hooks enabled");
	MH_EnableHook(MH_ALL_HOOKS);
	
}

void Hooks::GameMode_tick(C_GameMode* _this) {
	static auto oTick = g_Hooks.GameMode_tickHook->GetFastcall<void, C_GameMode*>();
	oTick(_this);

	GameData::updateGameData(_this);
	if (_this->player == g_Data.getLocalPlayer() && _this->player != nullptr) {
		moduleMgr->onTick(_this);
	}
}

void Hooks::SurvivalMode_tick(C_GameMode* _this) {
	static auto oTick = g_Hooks.SurvivalMode_tickHook->GetFastcall<void, C_GameMode*>();
	oTick(_this);
	GameData::updateGameData(_this);
	if (_this->player == g_Data.getLocalPlayer() && _this->player != nullptr) {
		moduleMgr->onTick(_this);
	}
}

void Hooks::ChatScreenController_sendChatMessage(uint8_t* _this) {
	static auto oSendMessage = g_Hooks.ChatScreenController_sendChatMessageHook->GetFastcall<void, void*>();

	using addCommandToChatHistory_t = void(__fastcall*)(uint8_t*, TextHolder*);
	static addCommandToChatHistory_t addCommandToChatHistory = reinterpret_cast<addCommandToChatHistory_t>(FindSignature("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC ?? 48 8B 99 ?? ?? ?? ?? 48 8B F2 80"));

	TextHolder* messageHolder = reinterpret_cast<TextHolder*>(_this + 0xA70);
	if (messageHolder->getTextLength() > 0) {
		char* message = messageHolder->getText();

		if (*message == cmdMgr->prefix) {
			cmdMgr->execute(message);

			addCommandToChatHistory(_this, messageHolder);  // This will put the command in the chat history (Arrow up/down)

			__int64 v17 = 0;
			__int64* v15 = *(__int64**)(*(__int64*)(_this + 0xA58) + 0x20i64);
			__int64 v16 = *v15;

			if (*(BYTE*)(_this + 0xA9A))
				v17 = (*(__int64(__cdecl**)(__int64*))(v16 + 0x990))(v15);
			else
				v17 = (*(__int64(__cdecl**)(__int64*))(v16 + 0x988))(v15);
			*(DWORD*)(_this + 0xA94) = *(DWORD*)(v17 + 0x20);

			messageHolder->resetWithoutDelete();
			return;
		} else if (*message == '.') {
			// maybe the user forgot his prefix, give him some helpful advice
			static bool helpedUser = false;
			if (!helpedUser) {
				helpedUser = true;
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sYour Horion prefix is: \"%s%c%s\"", RED, YELLOW, cmdMgr->prefix, RED);
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sEnter \"%s%cprefix .%s\" to reset your prefix", RED, YELLOW, cmdMgr->prefix, RED);
			}
		}
	}
	oSendMessage(_this);
}

__int64 Hooks::UIScene_setupAndRender(C_UIScene* uiscene, __int64 screencontext) {
	static auto oSetup = g_Hooks.UIScene_setupAndRenderHook->GetFastcall<__int64, C_UIScene*, __int64>();

	g_Hooks.shouldRender = uiscene->isPlayScreen();

	return oSetup(uiscene, screencontext);
}

__int64 Hooks::UIScene_render(C_UIScene* uiscene, __int64 screencontext) {
	static auto oRender = g_Hooks.UIScene_renderHook->GetFastcall<__int64, C_UIScene*, __int64>();

	g_Hooks.shouldRender = uiscene->isPlayScreen();

	bool alwaysRender = moduleMgr->isInitialized() && moduleMgr->getModule<HudModule>()->alwaysShow;

	TextHolder alloc = {};
	uiscene->getScreenName(&alloc);

	if (alloc.getTextLength() < 100) {
		strcpy_s(g_Hooks.currentScreenName, alloc.getText());
	}
	
	if (!g_Hooks.shouldRender) {
		g_Hooks.shouldRender = alwaysRender || (strcmp(alloc.getText(), "start_screen") == 0 || (alloc.getTextLength() >= 11 && strncmp(alloc.getText(), "play_screen", 11)) == 0);
	}
	alloc.resetWithoutDelete();

	return oRender(uiscene, screencontext);
}

__int64 Hooks::RenderText(__int64 a1, C_MinecraftUIRenderContext* renderCtx) {
	static auto oText = g_Hooks.RenderTextHook->GetFastcall<__int64, __int64, C_MinecraftUIRenderContext*>();
	C_GuiData* dat = g_Data.getClientInstance()->getGuiData();

	DrawUtils::setCtx(renderCtx, dat);

	{
		static bool wasConnectedBefore = false;
		static LARGE_INTEGER start;
		static LARGE_INTEGER frequency;
		if (frequency.QuadPart == 0) {
			QueryPerformanceFrequency(&frequency);
			QueryPerformanceCounter(&start);
		}
		static bool hasSentWarning = false;
		if (!g_Data.isInjectorConnectionActive() && !hasSentWarning) {
			
			__int64 retval = oText(a1, renderCtx);

			LARGE_INTEGER end, elapsed;
			QueryPerformanceCounter(&end);
			elapsed.QuadPart = end.QuadPart - start.QuadPart;
			float elapsedFlot = (float)elapsed.QuadPart / frequency.QuadPart;
			if (elapsedFlot > 1.5f && !hasSentWarning) {
				hasSentWarning = true;
				auto box = g_Data.addInfoBox("Warning", "Your injector doesn't seem to connect to Horion correctly.\nYou can ignore this, but some features may not work as expected.");
				box->closeTimer = 5;
				/*vec2_t windowSize = dat->windowSize;

				DrawUtils::fillRectangle(vec4_t(0, 0, windowSize.x, windowSize.y), MC_Color(0.2f, 0.2f, 0.2f), 0.8f);

				std::string text = "Download the new injector at http://horionbeta.club/";
				if (!wasConnectedBefore)
					DrawUtils::drawText(vec2_t(windowSize.x / 2 - DrawUtils::getTextWidth(&text, 1.5f) / 2, windowSize.y * 0.4f), &text, MC_Color(), 1.5f);
				text = "Remember to keep the injector open while playing";
				DrawUtils::drawText(vec2_t(windowSize.x / 2 - DrawUtils::getTextWidth(&text, wasConnectedBefore ? 1.5f : 0.7f) / 2, windowSize.y * (wasConnectedBefore ? 0.5f : 0.7f)), &text, MC_Color(), wasConnectedBefore ? 1.5f : 0.7f);
				text = "Uninject by holding down CTRL + L";
				DrawUtils::drawText(vec2_t(windowSize.x / 2 - DrawUtils::getTextWidth(&text, 0.7f) / 2, windowSize.y * 0.8f), &text, MC_Color(), 0.7f);

				DrawUtils::flush();*/
			}

			if(!hasSentWarning) // Wait for injector, it might connect in time
				return retval;
		} else
			wasConnectedBefore = true;
	}

	if (GameData::shouldHide() || !g_Hooks.shouldRender || !moduleMgr->isInitialized())
		return oText(a1, renderCtx);

	static auto hudModule = moduleMgr->getModule<HudModule>();
	static auto clickGuiModule = moduleMgr->getModule<ClickGuiMod>();

	HImGui.startFrame();

	g_Data.frameCount++;

	auto wid = g_Data.getClientInstance()->getGuiData()->windowSize;

	// Call PreRender() functions
	moduleMgr->onPreRender(renderCtx);
	DrawUtils::flush();

	__int64 retval = oText(a1, renderCtx);

	bool shouldPostRender = true;
	bool shouldRenderArrayList = true;
	bool shouldRenderTabGui = true;
	bool shouldRenderWatermark = true;

	static float rcolors[4];          // Rainbow color array RGBA
	static float disabledRcolors[4];  // Rainbow Colors, but for disabled modules
	static float currColor[4];        // ArrayList colors

	// Rainbow color updates
	{
		Utils::ApplyRainbow(rcolors);  // Increase Hue of rainbow color array
		disabledRcolors[0] = std::min(1.f, rcolors[0] * 0.4f + 0.2f);
		disabledRcolors[1] = std::min(1.f, rcolors[1] * 0.4f + 0.2f);
		disabledRcolors[2] = std::min(1.f, rcolors[2] * 0.4f + 0.2f);
		disabledRcolors[3] = 1;
	}

	{
		// Main Menu
		std::string screenName(g_Hooks.currentScreenName);
		if (strcmp(screenName.c_str(), "start_screen") == 0) {
			// Draw BIG epic horion watermark
			/*{
				std::string text = "H O R I O N";
				vec2_t textPos = vec2_t(wid.x / 2.f - DrawUtils::getTextWidth(&text, 8.f) / 2.f, wid.y / 9.5f);
				vec4_t rectPos = vec4_t(textPos.x - 55.f, textPos.y - 15.f, textPos.x + DrawUtils::getTextWidth(&text, 8.f) + 55.f, textPos.y + 75.f);
				DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48, 1), 1.f);
				DrawUtils::drawRectangle(rectPos, rcolors, 1.f, 2.f);
				DrawUtils::drawText(textPos, &text, MC_Color(255, 255, 255, 1), 8.f);
			}*/

			// Draw Custom Geo Button
			if (g_Data.allowWIPFeatures() && g_Data.isInjectorConnectionActive()) {
				if (HImGui.Button("Load Script Folder", vec2_t(wid.x * (0.765f - 0.5f), wid.y * 0.92f), true)) {
					HorionDataPacket packet;
					packet.cmd = CMD_FOLDERCHOOSER;
					auto tmp = std::shared_ptr<unsigned char[]>(new unsigned char[300]);
					packet.data.swap(tmp);
					memset(packet.data.get(), 0, 300);
					strcpy_s((char*)packet.data.get(), 200, "{\"title\": \"Select a Script Folder\", \"filter\":\".js\"}");
					packet.dataArraySize = (int)strlen((char*)packet.data.get());
					packet.params[0] = g_Data.addInjectorResponseCallback([](std::shared_ptr<HorionDataPacket> pk) {
						if (pk->params[0] != 1) {  // Dialog Canceled, reset geo
							auto box = g_Data.addInfoBox("Scripting", "Invalid Folder");
							box->closeTimer = 1;
							return;
						}

						wchar_t* jsonData = reinterpret_cast<wchar_t*>(pk->data.get());
						std::wstring jsonDataStr(jsonData);

						json parsed = json::parse(jsonDataStr);
						if (parsed["path"].is_string()) {
							auto box = g_Data.addInfoBox("Importing Script", "Please wait...");
							std::thread gamer([parsed, box]() {
								auto result = scriptMgr.importScriptFolder(parsed["path"].get<std::string>());
								if (result)
									box->fadeTarget = 0;
								else {
									box->message = "Script import error, \ncheck the console";
									box->closeTimer = 2;
								}
							});
							gamer.detach();
						}
					});

					g_Data.sendPacketToInjector(packet);
				}
				if (HImGui.Button("Custom Geometry", vec2_t(wid.x * 0.765f, wid.y * 0.92f), true)) {
					HorionDataPacket packet;
					packet.cmd = CMD_FILECHOOSER;
					auto tmp = std::shared_ptr<unsigned char[]>(new unsigned char[300]);
					packet.data.swap(tmp);
					memset(packet.data.get(), 0, 300);
					strcpy_s((char*)packet.data.get(), 200, "{\"title\": \"Select a 3d object\", \"filter\":\"Object Files (*.obj)|*.obj\"}");
					packet.dataArraySize = (int)strlen((char*)packet.data.get());
					packet.params[0] = g_Data.addInjectorResponseCallback([](std::shared_ptr<HorionDataPacket> pk) {
						if (pk->params[0] != 1 && std::get<0>(g_Data.getCustomGeoOverride())) {  // Dialog Canceled, reset geo
							auto box = g_Data.addInfoBox("Geometry reset", "Geometry override removed");
							box->closeTimer = 1;
							return;
						}

						wchar_t* jsonData = reinterpret_cast<wchar_t*>(pk->data.get());
						std::wstring jsonDataStr(jsonData);

						json parsed = json::parse(jsonDataStr);
						if (parsed["path"].is_string()) {
							auto box = g_Data.addInfoBox("Importing Skin", "Please wait...");
							std::thread gamer([parsed, box]() {
								SkinUtil::importGeo(Utils::stringToWstring(parsed["path"].get<std::string>()));
								box->fadeTarget = 0;
							});
							gamer.detach();
						}
					});

					g_Data.sendPacketToInjector(packet);
				}
				if (HImGui.Button("Custom Texture", vec2_t(wid.x * 0.5f, wid.y * 0.92f), true)) {
					HorionDataPacket packet;
					packet.cmd = CMD_FILECHOOSER;
					auto tmp = std::shared_ptr<unsigned char[]>(new unsigned char[500]);
					packet.data.swap(tmp);
					memset(packet.data.get(), 0, 500);
					strcpy_s((char*)packet.data.get(), 400, "{\"title\": \"Select a raw image file\", \"filter\":\"Raw image files (*.data, *.raw)|*.data;*.raw\"}");
					packet.dataArraySize = (int)strlen((char*)packet.data.get());
					packet.params[0] = g_Data.addInjectorResponseCallback([](std::shared_ptr<HorionDataPacket> pk) {
						if (pk->params[0] != 1 && std::get<0>(g_Data.getCustomTextureOverride())) {  // Dialog Canceled, reset texture
							auto box = g_Data.addInfoBox("Texture reset", "Texture override removed");
							box->closeTimer = 1;
							return;
						}

						wchar_t* jsonData = reinterpret_cast<wchar_t*>(pk->data.get());
						std::wstring jsonDataStr(jsonData);

						json parsed = json::parse(jsonDataStr);
						if (parsed["path"].is_string()) {
							auto box = g_Data.addInfoBox("Importing texture...", "");
							std::thread gamer([parsed, box]() {
								auto contents = Utils::readFileContents(Utils::stringToWstring(parsed["path"].get<std::string>()));
								if (contents.size() > 0) {
									auto texturePtr = std::shared_ptr<unsigned char[]>(new unsigned char[contents.size() + 1]);
									memcpy(texturePtr.get(), contents.c_str(), contents.size());
									texturePtr.get()[contents.size()] = 0;
									g_Data.setCustomTextureOverride(true, std::make_shared<std::tuple<std::shared_ptr<unsigned char[]>, size_t>>(texturePtr, contents.size()));
									box->title = "Success";
									box->closeTimer = 0.3f;
								} else {
									box->title = "Error!";
									box->message = "Could not read texture file (empty?)";
									box->closeTimer = 2.f;
								}
							});
							gamer.detach();
						}
					});

					g_Data.sendPacketToInjector(packet);
				}
			}

		} else {
			shouldRenderTabGui = hudModule->tabgui && hudModule->isEnabled();
			shouldRenderArrayList = hudModule->arraylist && hudModule->isEnabled();
			shouldRenderWatermark = hudModule->watermark && hudModule->isEnabled();

			if (clickGuiModule->isEnabled()) {
				ClickGui::render();
				shouldPostRender = false;
				shouldRenderArrayList = false;
				shouldRenderTabGui = false;
				shouldRenderWatermark = false;
			}

			if (shouldRenderTabGui) TabGui::render();

			{
				// Display ArrayList on the Right?
				static constexpr bool isOnRightSide = true;

				float yOffset = 0;  // Offset of next Text
				vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
				vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;

				vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();
				mousePos.div(windowSizeReal);
				mousePos.mul(windowSize);

				// Draw Horion logo
				if (shouldRenderWatermark) {
					constexpr float nameTextSize = 1.5f;
					constexpr float versionTextSize = 0.7f;
					static const float textHeight = (nameTextSize + versionTextSize * 0.7f /* We don't quite want the version string in its own line, just a bit below the name */) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
					constexpr float borderPadding = 1;
					constexpr float margin = 5;

					static std::string name = "Horion";
#ifdef _DEBUG
					static std::string version = "dev";
#elif defined _BETA
					static std::string version = "beta";
#else
					static std::string version = "public";
#endif

					float nameLength = DrawUtils::getTextWidth(&name, nameTextSize);
					float fullTextLength = nameLength + DrawUtils::getTextWidth(&version, versionTextSize);
					vec4_t rect = vec4_t(
						windowSize.x - margin - fullTextLength - borderPadding * 2,
						windowSize.y - margin - textHeight,
						windowSize.x - margin + borderPadding,
						windowSize.y - margin);

					DrawUtils::drawRectangle(rect, MC_Color(13, 29, 48), 1.f, 2.f);
					DrawUtils::fillRectangle(rect, MC_Color(rcolors), 1.f);
					DrawUtils::drawText(vec2_t(rect.x + borderPadding, rect.y), &name, MC_Color(6, 15, 24), nameTextSize);
					DrawUtils::drawText(vec2_t(rect.x + borderPadding + nameLength, rect.w - 7), &version, MC_Color(0, 0, 0), versionTextSize);
				}

				// Draw ArrayList
				if (moduleMgr->isInitialized() && shouldRenderArrayList) {

					// Parameters
					float textSize = hudModule->scale;
					float textPadding = 1.0f * textSize;
					float textHeight = 10.0f * textSize;
					float smoothness = 2;

					struct IModuleContainer {
						// Struct used to Sort IModules in a std::set
						std::shared_ptr<IModule> backingModule;
						std::string moduleName;
						bool enabled;
						int keybind;
						float textWidth;
						vec2_t* pos;
						bool shouldRender = true;

						IModuleContainer(std::shared_ptr<IModule> mod) {
							const char* moduleNameChr = mod->getModuleName();
							this->enabled = mod->isEnabled();
							this->keybind = mod->getKeybind();
							this->backingModule = mod;
							this->pos = mod->getPos();

							if (keybind == 0x0)
								moduleName = moduleNameChr;
							else {
								char text[50];
								sprintf_s(text, 50, "%s%s", moduleNameChr, hudModule->keybinds ? std::string(" [" + std::string(Utils::getKeybindName(keybind)) + "]").c_str() : "");
								moduleName = text;
							}

							if (!this->enabled && *this->pos == vec2_t(0.f, 0.f))
								this->shouldRender = false;
							this->textWidth = DrawUtils::getTextWidth(&moduleName, hudModule->scale);
						}

						bool operator<(const IModuleContainer& other) const {
							/*if (enabled) {
						if (!other.enabled)  // We are enabled
							return true;
					} else if (other.enabled)  // They are enabled
						return false;*/

							if (this->textWidth == other.textWidth)
								return moduleName < other.moduleName;
							return this->textWidth > other.textWidth;
						}
					};

					// Mouse click detector
					static bool wasLeftMouseDown = GameData::isLeftClickDown();  // Last isDown value
					bool leftMouseDown = GameData::isLeftClickDown();            // current isDown value

					bool executeClick = leftMouseDown && leftMouseDown != wasLeftMouseDown;  // isDown == true AND (current state IS NOT last state)
					wasLeftMouseDown = leftMouseDown;                                        // Set last isDown value

					std::set<IModuleContainer> modContainerList;
					// Fill modContainerList with Modules
					{
						auto lock = moduleMgr->lockModuleList();
						std::vector<std::shared_ptr<IModule>>* moduleList = moduleMgr->getModuleList();
						for (auto it : *moduleList) {
							if (it.get() != hudModule)
								modContainerList.emplace(IModuleContainer(it));
						}
					}

					int a = 0;
					int b = 0;
					int c = 0;

					// Loop through mods to display Labels
					for (std::set<IModuleContainer>::iterator it = modContainerList.begin(); it != modContainerList.end(); ++it) {
						if (!it->shouldRender)
							continue;

						std::string textStr = it->moduleName;
						float textWidth = it->textWidth;

						float xOffsetOri = windowSize.x - textWidth - (textPadding * 2);

						float xOffset = windowSize.x - it->pos->x;

						it->pos->x += smoothness;

						if (xOffset < xOffsetOri) {
							xOffset = xOffsetOri;
						}
						if (!it->enabled) {
							xOffset += it->pos->y;
							it->pos->y += smoothness;
						}
						if (xOffset >= windowSize.x && !it->enabled) {
							it->pos->x = 0.f;
							it->pos->y = 0.f;
						}

						vec2_t textPos = vec2_t(
							xOffset + textPadding,
							yOffset + textPadding);
						vec4_t rectPos = vec4_t(
							xOffset - 2,
							yOffset,
							isOnRightSide ? windowSize.x : textWidth + (textPadding * 2),
							yOffset + textPadding * 2 + textHeight);
						vec4_t leftRect = vec4_t(
							xOffset - 2,
							yOffset,
							xOffset - 1,
							yOffset + textPadding * 2 + textHeight);
						c++;
						b++;
						if (b < 20)
							a = moduleMgr->getEnabledModuleCount() * 2;
						else
							b = 0;
						currColor[3] = rcolors[3];
						Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], currColor[0], currColor[1], currColor[2]);
						currColor[0] += 1.f / a * c;
						Utils::ColorConvertHSVtoRGB(currColor[0], currColor[1], currColor[2], currColor[0], currColor[1], currColor[2]);

						DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), 1.f);
						DrawUtils::fillRectangle(leftRect, MC_Color(currColor), 1.f);
						if (!GameData::canUseMoveKeys() && rectPos.contains(&mousePos) && hudModule->clickToggle) {
							vec4_t selectedRect = rectPos;
							selectedRect.x = leftRect.z;
							if (leftMouseDown) {
								DrawUtils::fillRectangle(selectedRect, MC_Color(0.8f, 0.8f, 0.8f), 0.8f);
								if (executeClick)
									it->backingModule->toggle();
							} else
								DrawUtils::fillRectangle(selectedRect, MC_Color(0.8f, 0.8f, 0.8f, 0.8f), 0.3f);
						}
						DrawUtils::drawText(textPos, &textStr, MC_Color(currColor), textSize);

						yOffset += textHeight + (textPadding * 2);
					}
					c = 0;
					modContainerList.clear();
				}
			}
		}
	}

	// Zoom calc
	{
		static auto zoomModule = moduleMgr->getModule<Zoom>();
		if (zoomModule->isEnabled()) zoomModule->target = zoomModule->strength;
		zoomModule->modifier = zoomModule->target - ((zoomModule->target - zoomModule->modifier) * 0.8f);
		if (abs(zoomModule->modifier - zoomModule->target) < 0.1f && !zoomModule->isEnabled())
			zoomModule->zooming = false;
	}

	if (shouldPostRender) moduleMgr->onPostRender(renderCtx);
	HImGui.endFrame();
	DrawUtils::flush();

	// Draw Info / Alert Boxes
	{
		auto box = g_Data.getFreshInfoBox();
		if (box) {
			box->fade();
			if (box->fadeTarget == 1 && box->closeTimer <= 0 && box->closeTimer > -1)
				box->fadeTarget = 0;
			else if (box->closeTimer > 0 && box->fadeVal > 0.9f)
				box->closeTimer -= 1.f / 60;
			const float paddingHoriz = 40 * box->fadeVal;
			const float paddingVert = 10 * box->fadeVal;
			const float titleTextSize = box->fadeVal * 2;
			const float messageTextSize = box->fadeVal * 1;
			const float titleTextHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * titleTextSize;

			int lines = 1;
			std::string substring = box->message;
			while (lines < 5) {
				auto brea = substring.find("\n");
				if (brea == std::string::npos || brea + 1 >= substring.size())
					break;
				substring = substring.substr(brea + 1);
				lines++;
			}
			if (box->message.size() == 0)
				lines = 0;

			const float messageHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * messageTextSize * lines;

			float titleWidth = DrawUtils::getTextWidth(&box->title, titleTextSize);
			float msgWidth = DrawUtils::getTextWidth(&box->message, messageTextSize);
			vec2_t centerPos(wid.x / 2.f, wid.y / 9.f);
			vec2_t textPos = vec2_t(wid.x / 2.f - titleWidth / 2.f, wid.y / 9.f);
			vec2_t msgPos = vec2_t(wid.x / 2.f - msgWidth / 2.f, textPos.y + titleTextHeight + paddingVert);
			vec4_t rectPos = vec4_t(
				centerPos.x - paddingHoriz - std::max(titleWidth, msgWidth) / 2,
				centerPos.y - paddingVert,
				centerPos.x + paddingHoriz + std::max(titleWidth, msgWidth) / 2,
				centerPos.y + paddingVert * 2 + titleTextHeight + messageHeight * lines);
			DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), box->fadeVal);
			DrawUtils::drawRectangle(rectPos, rcolors, box->fadeVal, 2.f);
			DrawUtils::drawText(textPos, &box->title, MC_Color(), titleTextSize, box->fadeVal);
			DrawUtils::drawText(msgPos, &box->message, MC_Color(), messageTextSize, box->fadeVal);
		}
	}
	DrawUtils::flush();

	return retval;
}

float* Hooks::Dimension_getFogColor(__int64 _this, float* color, __int64 a3, float a4) {
	static auto oGetFogColor = g_Hooks.Dimension_getFogColorHook->GetFastcall<float*, __int64, float*, __int64, float>();

	static float rcolors[4];

	static auto nightMod = moduleMgr->getModule<NightMode>();
	if (nightMod->isEnabled()) {
		color[0] = 0.f;
		color[1] = 0.f;
		color[2] = 0.2f;
		color[3] = 1;
		return color;
	}

	static auto rainbowSkyMod = moduleMgr->getModule<RainbowSky>();
	if (rainbowSkyMod->isEnabled()) {
		if (rcolors[3] < 1) {
			rcolors[0] = 1;
			rcolors[1] = 0.2f;
			rcolors[2] = 0.2f;
			rcolors[3] = 1;
		}

		Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);  // perfect code, dont question this

		rcolors[0] += 0.001f;
		if (rcolors[0] >= 1)
			rcolors[0] = 0;

		Utils::ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);

		return rcolors;
	}
	return oGetFogColor(_this, color, a3, a4);
}

float Hooks::Dimension_getTimeOfDay(__int64 _this, int a2, float a3) {
	static auto oGetTimeOfDay = g_Hooks.Dimension_getTimeOfDayHook->GetFastcall<float, __int64, int, float>();

	static auto timeChange = moduleMgr->getModule<TimeChanger>();
	if (timeChange->isEnabled()) {
		return timeChange->modifier;
	}

	return oGetTimeOfDay(_this, a2, a3);
}

float Hooks::Dimension_getSunIntensity(__int64 a1, float a2, vec3_t* a3, float a4) {
	static auto oGetSunIntensity = g_Hooks.Dimension_getSunIntensityHook->GetFastcall<float, __int64, float, vec3_t*, float>();

	static auto nightMod = moduleMgr->getModule<NightMode>();
	if (nightMod->isEnabled()) {
		return -0.5f;
	}

	return oGetSunIntensity(a1, a2, a3, a4);
}

void Hooks::ChestBlockActor_tick(C_ChestBlockActor* _this, void* a) {
	static auto oTick = g_Hooks.ChestBlockActor_tickHook->GetFastcall<void, C_ChestBlockActor*, void*>();
	oTick(_this, a);
	if (_this != nullptr)
		GameData::addChestToList(_this);
}

void Hooks::Actor_lerpMotion(C_Entity* _this, vec3_t motVec) {
	static auto oLerp = g_Hooks.Actor_lerpMotionHook->GetFastcall<void, C_Entity*, vec3_t>();

	if (g_Data.getLocalPlayer() != _this)
		return oLerp(_this, motVec);

	static auto noKnockbackmod = moduleMgr->getModule<Velocity>();
	if (noKnockbackmod->isEnabled()) {
		static void* networkSender = nullptr;

		if (!networkSender) {
			if (g_Data.getVersion() == GAMEVERSION::g_1_16_0)
				networkSender = reinterpret_cast<void*>(6 + FindSignature("FF 90 ?? ?? ?? ?? 4C 8D 9C 24 ?? ?? ?? ?? 49 8B 5B 18 49 8B 73 28 49 8B E3 5F C3"));
			else
				networkSender = reinterpret_cast<void*>(3 + FindSignature("FF 50 ? 41 80 BE ? ? ? ? ? 0F 85 ? ? ? ? EB 76"));
		}
		
		if (networkSender == _ReturnAddress()) {
			motVec = _this->velocity.lerp(motVec, noKnockbackmod->xModifier, noKnockbackmod->yModifier, noKnockbackmod->xModifier);
		}
	}
	
	oLerp(_this, motVec);
}

int Hooks::AppPlatform_getGameEdition(__int64 _this) {
	static auto oGetEditon = g_Hooks.AppPlatform_getGameEditionHook->GetFastcall<signed int, __int64>();

	static auto mod = moduleMgr->getModule<EditionFaker>();
	if (mod->isEnabled()) {
		return mod->getFakedEditon();
	}

	return oGetEditon(_this);
}

void Hooks::PleaseAutoComplete(__int64 a1, __int64 a2, TextHolder* text, int a4) {
	static auto oAutoComplete = g_Hooks.PleaseAutoCompleteHook->GetFastcall<void, __int64, __int64, TextHolder*, int>();
	char* tx = text->getText();

	using syncShit_t = void(__fastcall*)(__int64*, TextHolder*);
	static syncShit_t syncShit = nullptr;
	if (syncShit == nullptr) {
		uintptr_t sigOffset = 0;
		// sig of function: (present 3 times in the exe)
		sigOffset = FindSignature("48 8B D7 48 8B 8B ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 9C") + 11;
		auto funcOffset = *reinterpret_cast<int*>(sigOffset);
		sigOffset += 4 + funcOffset;

		syncShit = reinterpret_cast<syncShit_t>(sigOffset);
	}

	if (tx != nullptr && text->getTextLength() >= 1 && tx[0] == '.') {
		std::string search = tx + 1;                                              // Dont include the '.'
		std::transform(search.begin(), search.end(), search.begin(), ::tolower);  // make the search text lowercase

		struct LilPlump {
			std::string cmdAlias;
			IMCCommand* command = 0;
			bool shouldReplace = true;  // Should replace the current text in the box (autocomplete)

			bool operator<(const LilPlump& o) const {
				return cmdAlias < o.cmdAlias;
			}
		};  // This is needed so the std::set sorts it alphabetically

		std::set<LilPlump> searchResults;

		std::vector<IMCCommand*>* commandList = cmdMgr->getCommandList();
		for (auto it = commandList->begin(); it != commandList->end(); ++it) {  // Loop through commands
			IMCCommand* c = *it;
			auto* aliasList = c->getAliasList();
			for (auto it = aliasList->begin(); it != aliasList->end(); ++it) {  // Loop through aliases
				std::string cmd = *it;
				LilPlump plump;

				for (size_t i = 0; i < search.size(); i++) {  // Loop through search string
					char car = search.at(i);
					if (car == ' ' && i == cmd.size()) {
						plump.shouldReplace = false;
						break;
					} else if (i >= cmd.size())
						goto outerContinue;

					if (car != cmd.at(i))  // and compare
						goto outerContinue;
				}
				// Not at outerContinue? Then we got a good result!
				{
					cmd.insert(0, 1, '.');  // Prepend the '.'

					plump.cmdAlias = cmd;
					plump.command = c;
					searchResults.emplace(plump);
				}

			outerContinue:
				continue;
			}
		}

		if (!searchResults.empty()) {
			LilPlump firstResult = *searchResults.begin();

			size_t maxReplaceLength = firstResult.cmdAlias.size();
			if (searchResults.size() > 1) {
				for (auto it = searchResults.begin()++; it != searchResults.end(); it++) {
					auto alias = it->cmdAlias;
					maxReplaceLength = std::min(maxReplaceLength, alias.size());

					for (int i = 0; i < maxReplaceLength; i++) {
						if (alias[i] != firstResult.cmdAlias[i]) {
							maxReplaceLength = i;
							break;
						}
					}
				}
			} else
				maxReplaceLength = firstResult.cmdAlias.size();

			g_Data.getGuiData()->displayClientMessageF("==========");
			if (searchResults.size() > 1) {
				for (auto it = searchResults.begin(); it != searchResults.end(); ++it) {
					LilPlump plump = *it;
					g_Data.getGuiData()->displayClientMessageF("%s%s - %s%s", plump.cmdAlias.c_str(), GRAY, ITALIC, plump.command->getDescription());
				}
			}
			if (firstResult.command->getUsage(firstResult.cmdAlias.c_str() + 1)[0] == 0)
				g_Data.getGuiData()->displayClientMessageF("%s%s %s- %s", WHITE, firstResult.cmdAlias.c_str(), GRAY, firstResult.command->getDescription());
			else
				g_Data.getGuiData()->displayClientMessageF("%s%s %s %s- %s", WHITE, firstResult.cmdAlias.c_str(), firstResult.command->getUsage(firstResult.cmdAlias.c_str() + 1 /*exclude prefix*/), GRAY, firstResult.command->getDescription());

			if (firstResult.shouldReplace) {
				if (search.size() == firstResult.cmdAlias.size() - 1 && searchResults.size() == 1) {
					maxReplaceLength++;
					firstResult.cmdAlias.append(" ");
				}

				text->setText(firstResult.cmdAlias.substr(0, maxReplaceLength));  // Set text
				// now sync with the UI thread
				syncShit(0, text);
			}
		}

		return;
	}
	oAutoComplete(a1, a2, text, a4);
}

void Hooks::LoopbackPacketSender_sendToServer(C_LoopbackPacketSender* a, C_Packet* packet) {
	static auto oFunc = g_Hooks.LoopbackPacketSender_sendToServerHook->GetFastcall<void, C_LoopbackPacketSender*, C_Packet*>();

	static auto autoSneakMod = moduleMgr->getModule<AutoSneak>();
	static auto freecamMod = moduleMgr->getModule<Freecam>();
	static auto blinkMod = moduleMgr->getModule<Blink>();
	static auto noPacketMod = moduleMgr->getModule<NoPacket>();

	if (noPacketMod->isEnabled() && g_Data.isInGame())
		return;

	if (freecamMod->isEnabled() || blinkMod->isEnabled()) {
		if (packet->isInstanceOf<C_MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>()) {
			if (blinkMod->isEnabled()) {
				if (packet->isInstanceOf<C_MovePlayerPacket>()) {
					C_MovePlayerPacket* meme = reinterpret_cast<C_MovePlayerPacket*>(packet);
					meme->onGround = true;                                                            //Don't take Fall Damages when turned off
					blinkMod->getMovePlayerPacketHolder()->push_back(new C_MovePlayerPacket(*meme));  // Saving the packets
				} else {
					blinkMod->getPlayerAuthInputPacketHolder()->push_back(new PlayerAuthInputPacket(*reinterpret_cast<PlayerAuthInputPacket*>(packet)));
				}
			}
			return;  // Dont call LoopbackPacketSender_sendToServer
		}
	} else if (!blinkMod->isEnabled()) {
		if (blinkMod->getMovePlayerPacketHolder()->size() > 0) {
			for (auto it : *blinkMod->getMovePlayerPacketHolder()) {
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getMovePlayerPacketHolder()->clear();
			return;
		}
		if (blinkMod->getPlayerAuthInputPacketHolder()->size() > 0) {
			for (auto it : *blinkMod->getPlayerAuthInputPacketHolder()) {
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getPlayerAuthInputPacketHolder()->clear();
			return;
		}
	}

	if (autoSneakMod->isEnabled() && g_Data.getLocalPlayer() != nullptr && autoSneakMod->doSilent && packet->isInstanceOf<C_PlayerActionPacket>()) {
		auto* pp = reinterpret_cast<C_PlayerActionPacket*>(packet);
		
		if (pp->action == 12 && pp->entityRuntimeId == g_Data.getLocalPlayer()->entityRuntimeId) 
			return; //dont send uncrouch
	}
	
	moduleMgr->onSendPacket(packet);

	if (strcmp(packet->getName()->getText(), "EmotePacket") == 0) {
		auto varInt = reinterpret_cast<__int64*>(reinterpret_cast<__int64>(packet) + 0x28);
		auto text = reinterpret_cast<TextHolder*>(reinterpret_cast<__int64>(packet) + 0x30);
		auto bet = reinterpret_cast<unsigned char*>(reinterpret_cast<__int64>(packet) + 0x50);
		logF("emote %llX %s %i", *varInt, text->getText(), *bet);
	}

	oFunc(a, packet);
}

float Hooks::LevelRendererPlayer_getFov(__int64 _this, float a2, bool a3) {
	static auto oGetFov = g_Hooks.LevelRendererPlayer_getFovHook->GetFastcall<float, __int64, float, bool>();
	static void* renderItemInHand = reinterpret_cast<void*>(FindSignature("0F 28 C8 F3 0F 59 0D ?? ?? ?? ?? 41 0F 28 F9"));

	static void* setupCamera = reinterpret_cast<void*>(FindSignature("44 0F 28 D8 F3 44 0F 59 1D ?? ?? ?? ?? 0F"));

	static auto zoomModule = moduleMgr->getModule<Zoom>();

	if (_ReturnAddress() == renderItemInHand) {
		return oGetFov(_this, a2, a3);
	}
	if (_ReturnAddress() == setupCamera) {
		g_Data.fov = -oGetFov(_this, a2, a3) + 110.f;
		if (moduleMgr->isInitialized()) {
			if (!zoomModule->smooth && zoomModule->isEnabled()) return -zoomModule->target + 110.f;
			if (zoomModule->smooth && zoomModule->zooming) return -zoomModule->modifier + 110.f;
		}
		return oGetFov(_this, a2, a3);
	}
#ifdef _DEBUG
	logF("LevelRendererPlayer_getFov Return Address: %llX", _ReturnAddress());
	__debugbreak();  // IF we reach here, a sig is broken
#endif
	return oGetFov(_this, a2, a3);
}

void Hooks::MultiLevelPlayer_tick(C_EntityList* _this) {
	static auto oTick = g_Hooks.MultiLevelPlayer_tickHook->GetFastcall<void, C_EntityList*>();
	oTick(_this);
	GameData::EntityList_tick(_this);
}

void Hooks::GameMode_startDestroyBlock(C_GameMode* _this, vec3_ti* a2, uint8_t face, void* a4, void* a5) {
	static auto oFunc = g_Hooks.GameMode_startDestroyBlockHook->GetFastcall<void, C_GameMode*, vec3_ti*, uint8_t, void*, void*>();

	static auto nukerModule = moduleMgr->getModule<Nuker>();
	static auto instaBreakModule = moduleMgr->getModule<InstaBreak>();

	if (nukerModule->isEnabled()) {
		vec3_ti tempPos;

		int range = nukerModule->getNukerRadius();
		const bool isVeinMiner = nukerModule->isVeinMiner();
		const bool isAutoMode = nukerModule->isAutoMode();

		C_BlockSource* region = g_Data.getLocalPlayer()->region;
		auto selectedBlockId = ((region->getBlock(*a2)->blockLegacy))->blockId;
		uint8_t selectedBlockData = region->getBlock(*a2)->data;

		if (!isAutoMode) {
			for (int x = -range; x < range; x++) {
				for (int y = -range; y < range; y++) {
					for (int z = -range; z < range; z++) {
						tempPos.x = a2->x + x;
						tempPos.y = a2->y + y;
						tempPos.z = a2->z + z;
						if (tempPos.y > 0) {
							C_Block* blok = region->getBlock(tempPos);
							uint8_t data = blok->data;
							auto id = blok->blockLegacy->blockId;
							if (blok->blockLegacy->material->isSolid == true && (!isVeinMiner || (id == selectedBlockId && data == selectedBlockData)))
								_this->destroyBlock(&tempPos, face);
						}
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

	oFunc(_this, a2, face, a4, a5);
}

void Hooks::HIDController_keyMouse(C_HIDController* _this, void* a2, void* a3) {
	static auto oFunc = g_Hooks.HIDController_keyMouseHook->GetFastcall<void, C_HIDController*, void*, void*>();
	GameData::setHIDController(_this);
	isTicked = true;
	oFunc(_this, a2, a3);
	return;
}

int Hooks::BlockLegacy_getRenderLayer(C_BlockLegacy* a1) {
	static auto oFunc = g_Hooks.BlockLegacy_getRenderLayerHook->GetFastcall<int, C_BlockLegacy*>();

	static auto xrayMod = moduleMgr->getModule<Xray>();
	if (xrayMod->isEnabled()) {
		char* text = a1->name.getText();
		if (strstr(text, "ore") == NULL)
			if (strcmp(text, "lava") != NULL)
				if (strcmp(text, "water") != NULL)
					if (strcmp(text, "portal") != NULL)
						if (strcmp(text, "ancient_debris") != NULL)
							if (strcmp(text, "command_block") != NULL)
								return 10;
	}
	return oFunc(a1);
}

__int8* Hooks::BlockLegacy_getLightEmission(C_BlockLegacy* a1, __int8* a2) {
	static auto oFunc = g_Hooks.BlockLegacy_getLightEmissionHook->GetFastcall<__int8*, C_BlockLegacy*, __int8*>();

	static auto xrayMod = moduleMgr->getModule<Xray>();
	if (xrayMod->isEnabled()) {
		*a2 = 15;
		return a2;
	}
	return oFunc(a1, a2);
}

__int64 Hooks::LevelRenderer_renderLevel(__int64 _this, __int64 a2, __int64 a3) {
	static auto oFunc = g_Hooks.LevelRenderer_renderLevelHook->GetFastcall<__int64, __int64, __int64, __int64>();

	using reloadShit_t = void(__fastcall*)(__int64);
	static reloadShit_t reloadChunk = nullptr;

	if (!reloadChunk) {
		// RenderChunkCoordinator::rebuildAllRenderChunkGeometry
		reloadChunk = reinterpret_cast<reloadShit_t>(FindSignature("48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B F9 48 8D A9"));
	}

	static auto xrayMod = moduleMgr->getModule<Xray>();

	static bool lastState = false;
	if (lastState != xrayMod->isEnabled()) {
		// LevelRenderer::rebuildAllRenderChunkGeometry
		lastState = xrayMod->isEnabled();
		unsigned long long* v5;  // rdi
		unsigned long long* i;   // rbx

		v5 = *(unsigned long long**)(_this + 32);
		for (i = (unsigned long long*)*v5; i != v5; i = (unsigned long long*)*i)
			reloadChunk(i[3]);
	}

	auto ret = oFunc(_this, a2, a3);

	DrawUtils::setGameRenderContext(a2);
	moduleMgr->onLevelRender();
	DrawUtils::setGameRenderContext(0);

	return ret;
}

void Hooks::ClickFunc(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8) {
	static auto oFunc = g_Hooks.ClickFuncHook->GetFastcall<void, __int64, char, char, __int16, __int16, __int16, __int16, char>();
	static auto clickGuiModule = moduleMgr->getModule<ClickGuiMod>();

	//MouseButtons
	//0 = mouse move
	//1 = left click
	//2 = right click
	//3 = middle click
	//4 = scroll   (isDown: 120 (SCROLL UP) and -120 (SCROLL DOWN))

	ClickGui::onMouseClickUpdate((int)mouseButton, isDown);
	HImGui.onMouseClickUpdate((int)mouseButton, isDown);

	if (isDown)
		if (mouseButton == 1)
			g_Data.leftclickCount++;
		else if (mouseButton == 2)
			g_Data.rightclickCount++;

	if (clickGuiModule->isEnabled()) {
		if (mouseButton == 4) {
			ClickGui::onWheelScroll(isDown > 0);
		}
		if (mouseButton != 0)  // Mouse click event
			return;
	}
	return oFunc(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
}

__int64 Hooks::MoveInputHandler_tick(C_MoveInputHandler* a1, C_Entity* a2) {
	static auto oTick = g_Hooks.MoveInputHandler_tickHook->GetFastcall<__int64, C_MoveInputHandler*, C_Entity*>();

	auto ret = oTick(a1, a2);
	moduleMgr->onMove(a1);

	return 0;
}

__int64 Hooks::ChestScreenController_tick(C_ChestScreenController* a1) {
	static auto oFunc = g_Hooks.ChestScreenController_tickHook->GetFastcall<__int64, C_ChestScreenController*>();

	static auto chestStealerMod = moduleMgr->getModule<ChestStealer>();
	if(chestStealerMod->isEnabled()) chestStealerMod->chestScreenController_tick(a1);

	return oFunc(a1);
}

float Hooks::GetGamma(uintptr_t* a1) {
	static auto fullBrightModule = moduleMgr->getModule<FullBright>();
	if (fullBrightModule->isEnabled())
		return 25.f;

	//Get the normal gamma value
	float* gamer = (float*)*(a1 + 0x27);
	return *(gamer + 0x7A);
}

bool Hooks::Actor_isInWater(C_Entity* _this) {
	static auto oFunc = g_Hooks.Actor_isInWaterHook->GetFastcall<bool, C_Entity*>();

	if (g_Data.getLocalPlayer() != _this)
		return oFunc(_this);

	static auto airSwimModule = moduleMgr->getModule<AirSwim>();
	if (airSwimModule->isEnabled())
		return true;

	return oFunc(_this);
}

void Hooks::JumpPower(C_Entity* a1, float a2) {
	static auto oFunc = g_Hooks.JumpPowerHook->GetFastcall<void, C_Entity*, float>();
	static auto highJumpMod = moduleMgr->getModule<HighJump>();
	if (highJumpMod->isEnabled() && g_Data.getLocalPlayer() == a1) {
		a1->velocity.y = highJumpMod->jumpPower;
		return;
	}
	oFunc(a1, a2);
}

__int64 Hooks::MinecraftGame_onAppSuspended(__int64 _this) {
	static auto oFunc = g_Hooks.MinecraftGame_onAppSuspendedHook->GetFastcall<__int64, __int64>();
	configMgr->saveConfig();
	return oFunc(_this);
}

void Hooks::Actor_ascendLadder(C_Entity* _this) {
	static auto oFunc = g_Hooks.Actor_ascendLadderHook->GetFastcall<void, C_Entity*>();

	static auto fastLadderModule = moduleMgr->getModule<FastLadder>();
	if (fastLadderModule->isEnabled() && g_Data.getLocalPlayer() == _this) {
		_this->velocity.y = 0.6f;
		return;
	}
	return oFunc(_this);
}

void Hooks::Actor_startSwimming(C_Entity* _this) {
	static auto oFunc = g_Hooks.Actor_startSwimmingHook->GetFastcall<void, C_Entity*>();

	static auto jesusModule = moduleMgr->getModule<Jesus>();
	if (jesusModule->isEnabled() && g_Data.getLocalPlayer() == _this)
		return;

	oFunc(_this);
}

void Hooks::RakNetInstance_tick(C_RakNetInstance* _this, __int64 a2, __int64 a3) {
	static auto oTick = g_Hooks.RakNetInstance_tickHook->GetFastcall<void, C_RakNetInstance*, __int64, __int64>();
	GameData::setRakNetInstance(_this);
	oTick(_this, a2, a3);
}

float Hooks::GameMode_getPickRange(C_GameMode* _this, __int64 a2, char a3) {
	static auto oFunc = g_Hooks.GameMode_getPickRangeHook->GetFastcall<float, C_GameMode*, __int64, char>();

	if (g_Data.getLocalPlayer() != nullptr) {
		static auto extendedBlockReachModule = moduleMgr->getModule<ExtendedBlockReach>();
		if (extendedBlockReachModule->isEnabled())
			return extendedBlockReachModule->getBlockReach();

		static auto teleportModule = moduleMgr->getModule<Teleport>();
		if (teleportModule->isEnabled())
			return 255;
	}

	return oFunc(_this, a2, a3);
}

__int64 Hooks::ConnectionRequest_create(__int64 _this, __int64 privateKeyManager, void* a3, TextHolder* selfSignedId, TextHolder* serverAddress, __int64 clientRandomId, TextHolder* skinId, SkinData* skinData, __int64 capeData, CoolSkinData* coolSkinStuff, TextHolder* deviceId, int inputMode, int uiProfile, int guiScale, TextHolder* languageCode, bool sendEduModeParams, TextHolder* tenantId, __int64 unused, TextHolder* platformUserId, TextHolder* thirdPartyName, bool thirdPartyNameOnly, TextHolder* platformOnlineId, TextHolder* platformOfflineId, TextHolder* capeId) {
	static auto oFunc = g_Hooks.ConnectionRequest_createHook->GetFastcall<__int64, __int64, __int64, void*, TextHolder*, TextHolder*, __int64, TextHolder*, SkinData*, __int64, CoolSkinData*, TextHolder*, int, int, int, TextHolder*, bool, TextHolder*, __int64, TextHolder*, TextHolder*, bool, TextHolder*, TextHolder*, TextHolder*>();

	auto geoOverride = g_Data.getCustomGeoOverride();

	if (g_Data.allowWIPFeatures()) {
		logF("Connection Request: InputMode: %i UiProfile: %i GuiScale: %i", inputMode, uiProfile, guiScale);

		//Logger::WriteBigLogFileF(skinGeometryData->getTextLength() + 20, "Geometry: %s", skinGeometryData->getText());
		auto hResourceGeometry = FindResourceA((HMODULE)g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_TEXT1), "TEXT");
		auto hMemoryGeometry = LoadResource((HMODULE)g_Data.getDllModule(), hResourceGeometry);

		auto sizeGeometry = SizeofResource((HMODULE)g_Data.getDllModule(), hResourceGeometry);
		auto ptrGeometry = LockResource(hMemoryGeometry);

		auto hResourceSteve = FindResourceA((HMODULE)g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_STEVE), (char*)RT_RCDATA);
		auto hMemorySteve = LoadResource((HMODULE)g_Data.getDllModule(), hResourceSteve);

		auto sizeSteve = SizeofResource((HMODULE)g_Data.getDllModule(), hResourceSteve);
		auto ptrSteve = LockResource(hMemorySteve);

		//std::unique_ptr<TextHolder> newGeometryData(new TextHolder(ptrGeometry, sizeGeometry));
		TextHolder* newGeometryData = nullptr;

		if (std::get<0>(geoOverride)) {  // Is overriding geometry
			auto overrideGeo = std::get<1>(geoOverride);
			newGeometryData = new TextHolder(*overrideGeo.get());
		} else {  // Default Skin
				  /*char* str;  // Obj text
			{
				auto hResourceObj = FindResourceA(g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_OBJ), "TEXT");
				auto hMemoryObj = LoadResource(g_Data.getDllModule(), hResourceObj);

				auto sizeObj = SizeofResource(g_Data.getDllModule(), hResourceObj);
				auto ptrObj = LockResource(hMemoryObj);

				str = new char[sizeObj + 1];
				memset(str, 0, sizeObj + 1);
				memcpy(str, ptrObj, sizeObj);
			}

			newGeometryData = new TextHolder(SkinUtil::modGeometry(reinterpret_cast<char*>(ptrGeometry), SkinUtil::objToMesh(str)));*/
		}

		SkinData* newSkinData = new SkinData();
		newSkinData->SkinWidth = 128;
		newSkinData->SkinHeight = 128;
		newSkinData->skinData = ptrSteve;
		newSkinData->skinSize = sizeSteve;

		auto texOverride = g_Data.getCustomTextureOverride();
		auto texture = std::get<1>(texOverride);  // Put it here so it won't go out of scope until after it has been used
		if (std::get<0>(texOverride)) {           // Enabled
			newSkinData->skinData = std::get<0>(*texture.get()).get();
			newSkinData->skinSize = std::get<1>(*texture.get());
		}

		//Logger::WriteBigLogFileF(newGeometryData->getTextLength() + 20, "Geometry: %s", newGeometryData->getText());
		TextHolder* newSkinResourcePatch = new TextHolder(Utils::base64_decode("ewogICAiZ2VvbWV0cnkiIDogewogICAgICAiYW5pbWF0ZWRfZmFjZSIgOiAiZ2VvbWV0cnkuYW5pbWF0ZWRfZmFjZV9wZXJzb25hXzRjZGJiZmFjYTI0YTk2OGVfMF8wIiwKICAgICAgImRlZmF1bHQiIDogImdlb21ldHJ5LnBlcnNvbmFfNGNkYmJmYWNhMjRhOTY4ZV8wXzAiCiAgIH0KfQo="));

		TextHolder* fakeName = g_Data.getFakeName();
		TextHolder resourcePatchBackup;

		if (newGeometryData != nullptr) {
			memcpy(&resourcePatchBackup, &coolSkinStuff->skinResourcePatch, sizeof(TextHolder));
			memcpy(&coolSkinStuff->skinResourcePatch, newSkinResourcePatch, sizeof(TextHolder));
			styledReturnText = *newGeometryData;
			overrideStyledReturn = true;
		}

		//  newGeometryData == nullptr ? skinResourcePatch : newSkinResourcePatch, newGeometryData == nullptr ? skinGeometryData : newGeometryData, skinAnimationData, isPremiumSkin, isPersonaSkin,
		__int64 res = oFunc(_this, privateKeyManager, a3, selfSignedId, serverAddress, clientRandomId, skinId, (newGeometryData == nullptr && !std::get<0>(texOverride)) ? skinData : newSkinData, capeData, coolSkinStuff, deviceId, inputMode, uiProfile, guiScale, languageCode, sendEduModeParams, tenantId, unused, platformUserId, fakeName != nullptr ? fakeName : thirdPartyName, fakeName != nullptr ? true : thirdPartyNameOnly, platformOnlineId, platformOfflineId, capeId);

		if (newGeometryData != nullptr) {
			memcpy(&coolSkinStuff->skinResourcePatch, &resourcePatchBackup, sizeof(TextHolder));
			resourcePatchBackup.resetWithoutDelete();
		}
		overrideStyledReturn = false;

		styledReturnText = TextHolder();

		if (hMemoryGeometry)
			FreeResource(hMemoryGeometry);
		if (hMemorySteve)
			FreeResource(hMemorySteve);

		if (newGeometryData)
			delete newGeometryData;
		delete newSkinData;
		delete newSkinResourcePatch;
		return res;
	} else {
		TextHolder* fakeName = g_Data.getFakeName();
		__int64 res = oFunc(_this, privateKeyManager, a3, selfSignedId, serverAddress, clientRandomId, skinId, skinData, capeData, coolSkinStuff, deviceId, inputMode, uiProfile, guiScale, languageCode, sendEduModeParams, tenantId, unused, platformUserId, fakeName != nullptr ? fakeName : thirdPartyName, fakeName != nullptr ? true : thirdPartyNameOnly, platformOnlineId, platformOfflineId, capeId);
		return res;
	}
}

void Hooks::InventoryTransactionManager_addAction(C_InventoryTransactionManager* a1, C_InventoryAction* a2) {
	static auto Func = g_Hooks.InventoryTransactionManager_addActionHook->GetFastcall<void, C_InventoryTransactionManager*, C_InventoryAction*>();
	Func(a1, a2);
}

__int64 Hooks::PaintingRenderer__render(__int64 _this, __int64 a2, __int64 a3) {
	static auto Func = g_Hooks.PaintingRenderer__renderHook->GetFastcall<__int64, __int64, __int64, __int64>();

	static auto NoPaintingCrashMod = moduleMgr->getModule<NoPaintingCrash>();
	if (NoPaintingCrashMod->isEnabled())
		return 0;

	return Func(_this,a2,a3);
}

bool Hooks::DirectoryPackAccessStrategy__isTrusted(__int64 _this) {
	static auto func = g_Hooks.DirectoryPackAccessStrategy__isTrustedHook->GetFastcall<bool, __int64>();

	static uintptr_t** directoryPackAccessStrategyVtable = 0;

	if (!directoryPackAccessStrategyVtable) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 49 8D 76 50");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		directoryPackAccessStrategyVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
	}
	
	if (*reinterpret_cast<uintptr_t*>(_this) == (uintptr_t)directoryPackAccessStrategyVtable)
		return true;

	return func(_this);
}

bool Hooks::ReturnTrue(__int64 _this) {
	return true;
}

__int64 Hooks::SkinRepository___loadSkinPack(__int64 _this, __int64 pack, __int64 a3) {
	static auto func = g_Hooks.SkinRepository___loadSkinPackHook->GetFastcall<__int64, __int64, __int64, __int64>();

	//auto res = (*(unsigned __int8 (**)(void))(**(__int64**)(pack + 8) + 48i64))();
	//logF("SkinRepository___loadSkinPack: origin %i, is Trusted: %i", *(int*)((*(__int64*)pack) + 888i64), res);
	*(int*)((*(__int64*)pack) + 888i64) = 2;  // Set pack origin to "2"

	return func(_this, pack, a3);
}

GamerTextHolder* Hooks::toStyledString(__int64 strIn, GamerTextHolder* strOut) {
	static auto func = g_Hooks.toStyledStringHook->GetFastcall<GamerTextHolder*, __int64, GamerTextHolder*>();

	static auto conRequest = reinterpret_cast<__int64>(g_Hooks.ConnectionRequest_createHook->funcPtr);
	if (reinterpret_cast<__int64>(_ReturnAddress()) > conRequest && reinterpret_cast<__int64>(_ReturnAddress()) < conRequest + 10000 && overrideStyledReturn) {
		// Return address is within boundaries of connection request function

		strOut->copyFrom(&styledReturnText);

		return strOut;
	}

	return func(strIn, strOut);
}

void prepCoolBean() {
	if (g_Data.getClientInstance() && g_Data.getClientInstance()->minecraftGame->getServerEntries() && *reinterpret_cast<__int64*>(g_Data.getClientInstance()->minecraftGame->getServerEntries() + 0x50)) {
		auto serverEntries = g_Data.getClientInstance()->minecraftGame->getServerEntries() + 0x48;

		struct ThirdPartyServer {
			TextHolder serverName;
			TextHolder uuid;
			TextHolder masterPlayerAccount;
			TextHolder serverName2;
			TextHolder lobbyDescription;
			TextHolder domain;            // contains the last two parts of the domain .hivebedrock.network .mineplex.com
			TextHolder pathToServerIcon;  // C:\Users\user\AppData\Local\Packages\Microsoft.MinecraftUWP_8wekyb3d8bbwe\LocalCache\minecraftpe\ContentCache\ThirdPartyServer\\<file hash>.jpg
			TextHolder serverAddress;
			int coolBoye;

			ThirdPartyServer() {
				memset(this, 0, sizeof(ThirdPartyServer));
			}
		};

		struct BeansEntry {
			BeansEntry* nextEntry;
			BeansEntry* prevEntry;
			TextHolder masterPlayer;
			float unk;
			char filler[0x3c];
			TextHolder masterPlayer2;
			TextHolder serverName;
			char filler2[8];
			std::shared_ptr<ThirdPartyServer>* start;  // array
			std::shared_ptr<ThirdPartyServer>* end;    // end of array
		};

		auto listEnd = *reinterpret_cast<BeansEntry**>(serverEntries);

		auto current = listEnd;
		int count = 0;
		while (listEnd != current->nextEntry) {
			current = current->nextEntry;
			count++;
		}
		if (count > 5)  // we already added a server
			goto end;

		// make new one
		BeansEntry* epic = new BeansEntry();
		epic->nextEntry = listEnd;
		epic->prevEntry = current;
		epic->masterPlayer.setText("");
		epic->unk = current->unk;
		memcpy(epic->filler, current->filler, sizeof(BeansEntry::filler));
		epic->masterPlayer2.setText("");
		epic->serverName.setText("Epic");
		memcpy(epic->filler2, current->filler2, sizeof(BeansEntry::filler2));

		auto cT = current->start[0].get();

		std::shared_ptr<ThirdPartyServer>* start = new std::shared_ptr<ThirdPartyServer>[1];

		{
			ThirdPartyServer* t = new ThirdPartyServer();

			t->coolBoye = cT->coolBoye;
			t->uuid.setText("");
			t->masterPlayerAccount = cT->masterPlayerAccount;
			t->lobbyDescription = cT->lobbyDescription;
			t->pathToServerIcon.setText("");
			t->serverName.setText("Horion Server");
			t->serverName2.setText("Horion Server");  // This is the one actually displayed
			t->domain.setText(".horionbeta.club");
			t->serverAddress.setText("mc.horionbeta.club");
			start[0] = std::shared_ptr<ThirdPartyServer>(t);
		}

		epic->start = start;
		epic->end = &start[1];

		current->nextEntry = epic;

		// increase count
		*reinterpret_cast<__int64*>(g_Data.getClientInstance()->minecraftGame->getServerEntries() + 0x50) += 1;
	end:;
	}
}

__int64 Hooks::prepFeaturedServers(__int64 a1) {
	static auto func = g_Hooks.prepFeaturedServersHook->GetFastcall<__int64, __int64>();
	auto ret = func(a1);

	if (g_Data.getClientInstance() == nullptr)
		return ret;

	if (g_Data.allowWIPFeatures())
		prepCoolBean();

	return ret;
}

__int64 Hooks::prepFeaturedServersFirstTime(__int64 a1, __int64 a2) {
	static auto func = g_Hooks.prepFeaturedServersFirstTimeHook->GetFastcall<__int64, __int64, __int64>();

	if (g_Data.allowWIPFeatures())
		prepCoolBean();

	auto ret = func(a1, a2);

	return ret;
}

HRESULT Hooks::swapChain__present(IDXGISwapChain* chain, UINT syncInterval, UINT flags) {
	static auto func = g_Hooks.swapchain__presentHook->GetFastcall<HRESULT, IDXGISwapChain*, UINT, UINT>();

#if 0

#ifndef _D3DVECTOR
	typedef struct _D3DVECTOR3 {
		float x;
		float y;
		float z;

		_D3DVECTOR3(float x1, float y1, float z1) : x(x1), y(y1), z(z1) {}
		_D3DVECTOR3() {}
	} D3DVECTOR3;

	typedef struct _D3DVECTOR4 {
		float x;
		float y;
		float z;
		float w;

		_D3DVECTOR4(float x1, float y1, float z1, float w1) : x(x1), y(y1), z(z1), w(w1) {}
		_D3DVECTOR4() {}
	} D3DVECTOR4;
#endif
	struct VertexType {
		_D3DVECTOR4 position;
		_D3DVECTOR4 color;
	};


	static bool init = false;
	static ID3D11Device* device;
	static ID3D11DeviceContext* context;
	static ID3D11Buffer *vertexBuffer, *indexBuffer;
	static ID3D11InputLayout* m_pInputLayout;
	static ID3D11VertexShader* m_vertexShader;
	static VertexType *vertices;

	if (!init) {
		DXGI_SWAP_CHAIN_DESC desc;
		chain->GetDesc(&desc);

		chain->GetDevice(__uuidof(ID3D11Device), (void**)&device);

		device->GetImmediateContext(&context);

		D3D11_BUFFER_DESC bufferDesc;

		D3D11_INPUT_ELEMENT_DESC lineRectLayout[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};
			

		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = 50 * sizeof(VertexType);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;

		char* shader =
			"struct VS_INPUT \
		{														 \
			float4 vPosition : POSITION;										\
			 float4 color : COLOR;									\
		}; \
 \
		struct VS_OUTPUT { \
			float4 vPosition : SV_POSITION; \
			float4 vColor : COLOR;	\
		}; \
		VS_OUTPUT VSMain(VS_INPUT Input) { \
			VS_OUTPUT Output; \
 \
			Output.vPosition = Input.vPosition;\
			Output.vColor = Input.color; \
			return Output; \
		} \
		";

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
		ID3DBlob* shaderBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		auto res = D3DCompile(shader, strlen(shader), 0, 0, 0, "VSMain", "vs_4_0_level_9_1", flags, 0, &shaderBlob, &errorBlob);
		logF("result: %llX", res);
		if (!FAILED(res)) {
			// Create the vertex shader from the buffer.
			res = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &m_vertexShader);
			logF("vresult: %llX", res);
			if (!FAILED(res)) {
				res = device->CreateInputLayout(lineRectLayout, 2, shaderBlob->GetBufferPointer(),
												shaderBlob->GetBufferSize(), &m_pInputLayout);
				logF("vresult3 : %llX", res);
			}
		}

		{
			unsigned long* indices = new unsigned long[5];
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;

			//create the vertex array
			vertices = new VertexType[3];

			float left = 10 / (float)600, right = 500 / (float)60, top = 50 / (float)60, bottom = 500 / (float)60;

			//load the vertex array with data
			vertices[0].position = D3DVECTOR4(left, top, 0, 1);
			vertices[0].color = D3DVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
			vertices[1].position = D3DVECTOR4(right, top, 0, 1);
			vertices[1].color = D3DVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
			vertices[2].position = D3DVECTOR4(right, bottom, 0, 1);
			vertices[2].color = D3DVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
			//create the index array
			indices = new unsigned long[3];
			//load the index array with data
			for (int i = 0; i < 3; i++)
				indices[i] = i;

			HRESULT result;

			//set up the description of the dynamic vertex buffer
			vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;  //enables recreation and movement of vertices
			vertexBufferDesc.ByteWidth = sizeof(VertexType) * 3 ;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  //couples with dynamic
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;
			//give the subresource structure a pointer to the vertex data
			vertexData.pSysMem = vertices;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			//now create the vertex buffer
			result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
			if (FAILED(result)) {
				logF("CreateBuffer %llX", result);
			}

			//set up the description of the static index buffer
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(unsigned long) * 3;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;
			//give the subresource structure a pointer to the index data
			indexData.pSysMem = indices;
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			//create the index buffer
			result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
			if (FAILED(result)) {
				logF("CreateBuffer2 %llX", result);

			}
		}

		init = true;
	}

	/*
	
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	C_GuiData* dat = g_Data.getClientInstance()->getGuiData();
	if (dat) {
		io.DisplaySize = ImVec2(dat->windowSizeReal.x, dat->windowSizeReal.y);
	}

	ImGui::NewFrame();


	ImGui::Begin("bean");

	ImGui::Text("Hello");
	ImGui::Button("World!");

	ImGui::End();

	ImGui::Begin("wat");

	ImGui::Text("Hello");
	ImGui::Button("World!");

	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());*/

	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	//lock the vertex buffer so it can be written to
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	auto result = context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}

	auto verticesPtr = (VertexType*)mappedResource.pData;

	//copy the data into the vertex buffer
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * 3));

	context->Unmap(vertexBuffer, 0);

	context->VSSetShader(m_vertexShader, 0, 0);
	context->PSSetShader(0, 0, 0);
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(m_pInputLayout);
	
	context->DrawIndexed(3, 0, 0);
#endif

	//#define TEST_DIRECTX

#ifdef TEST_DIRECTX
	if (!GameWnd.isInitialized())
		GameWnd.Init(chain, g_Data.getDllModule());
	else
		GameWnd.Render();

#endif  // DEBUG
	return func(chain, syncInterval, flags);
}

HRESULT Hooks::swapChain__ResizeBuffers(IDXGISwapChain* chain, UINT bufferCount, UINT Width, UINT Height, DXGI_FORMAT Newformat, UINT SwapChainFlags) {
	auto func = g_Hooks.swapchain__resizeBuffersHook->GetFastcall<HRESULT, IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT>();

#ifdef TEST_DIRECTX
	GameWnd.OnWindowSizeChanged(static_cast<int>(Width), static_cast<int>(Height));
#endif
	HRESULT ret = func(chain, bufferCount, Width, Height, Newformat, SwapChainFlags);

	return ret;
}

__int64 Hooks::Cube__compile(__int64 a1, __int64 a2) {
	auto func = g_Hooks.cube__compileHook->GetFastcall<__int64, __int64, __int64>();

	auto ret = func(a1, a2);

	auto end = *reinterpret_cast<__int64*>(a1 + 0x38);
	auto it = *reinterpret_cast<__int64*>(a1 + 0x30);
	auto boi = it + 0x1C;
	while (it != end) {  // loop through PolygonQuad

		if (it != boi + 0x34) {
			auto iter2 = boi - 0xC;
			do {
				// PolygonQuad::compile
				float* floatyBoi = reinterpret_cast<float*>(iter2 - 16);
				logF("%.1f %.1f %.1f", floatyBoi[0], floatyBoi[1], floatyBoi[2]);
				iter2 += 0x14;

			} while (iter2 - 0x10 != boi + 0x34);
		}
		boi += 0x50;
		it += 0x50;
	}

	return ret;
}

__int64 Hooks::InGamePlayScreen___renderLevel(__int64 playScreen, __int64 a2, __int64 a3) {
	auto func = g_Hooks.InGamePlayScreen___renderLevelHook->GetFastcall<__int64, __int64, __int64, __int64>();
	return func(playScreen, a2, a3);
}
__int64 Hooks::GameMode_attack(C_GameMode* _this, C_Entity* ent) {
	auto func = g_Hooks.GameMode_attackHook->GetFastcall<__int64, C_GameMode*, C_Entity*>();
	moduleMgr->onAttack(ent);
	return func(_this, ent);
}
void Hooks::LocalPlayer__updateFromCamera(__int64 a1, C_Camera* camera) {
	auto func = g_Hooks.LocalPlayer__updateFromCameraHook->GetFastcall<__int64, __int64, C_Camera*>();
	auto freelookMod = moduleMgr->getModule<Freelook>();
	auto noHurtcamMod = moduleMgr->getModule<NoHurtcam>();

	if (freelookMod->redirectMouse) {
		freelookMod->cameraFacesFront = camera->facesPlayerFront;
		freelookMod->isThirdPerson = camera->renderPlayerModel;
		if (freelookMod->resetViewTick >= 0) {
			camera->setOrientationDeg(freelookMod->lastCameraAngle.x, freelookMod->lastCameraAngle.y, 0);
		} else {
			camera->getPlayerRotation(&freelookMod->lastCameraAngle);
		}

		return;
	}
	if (noHurtcamMod->isEnabled() && g_Data.isInGame() && g_Data.getLocalPlayer()->isAlive()) {
		vec2_t rot;
		camera->getPlayerRotation(&rot);
		if (camera->facesPlayerFront) {
			rot.x *= -1;  // rotate back
			rot.y += 180;
			rot = rot.normAngles();
		}

		camera->setOrientationDeg(rot.x, rot.y, 0);
	}

	func(a1, camera);
}
bool Hooks::Mob__isImmobile(C_Entity* ent) {
	auto func = g_Hooks.Mob__isImmobileHook->GetFastcall<bool, C_Entity*>();

	static auto antiImmobileMod = moduleMgr->getModule<AntiImmobile>();
	if (antiImmobileMod->isEnabled() && ent == g_Data.getLocalPlayer())
		return false;

	return func(ent);
}
void Hooks::InventoryTransactionManager__addAction(C_InventoryTransactionManager* _this, C_InventoryAction& action) {
	auto func = g_Hooks.InventoryTransactionManager__addActionHook->GetFastcall<void, C_InventoryTransactionManager*, C_InventoryAction&>();

#ifdef TEST_DEBUG
	char* srcName = "none";
	if (action.sourceItem.item && *action.sourceItem.item)
		srcName = (*action.sourceItem.item)->name.getText();
	char* targetName = "none";
	if (action.targetItem.item && *action.targetItem.item)
		targetName = (*action.targetItem.item)->name.getText();
	logF("%i %i %i %s %s", action.type, action.slot, action.sourceType, srcName, targetName, action.sourceType);

	/*if(/*action.slot == 14 && action.sourceType == 124 && strcmp(targetName, "none") == 0 && *strcmp(srcName, "stone_shovel") == 0){
		std::string tag = "{ench:[{id:9s,lvl:1s}]}";
		action.sourceItem.setUserData(std::move(Mojangson::parseTag(tag)));
	}
	if(/*action.slot == 2 && action.sourceType == 256 && strcmp(srcName, "none") == 0 &&* strcmp(targetName, "stone_shovel") == 0){
		std::string tag = "{ench:[{id:9s,lvl:1s}]}";
		action.targetItem.setUserData(std::move(Mojangson::parseTag(tag)));
	}*/
#endif

	func(_this, action);
}

void Hooks::LevelRendererPlayer__renderNameTags(__int64 a1, __int64 a2, TextHolder* a3, __int64 a4) {
	static auto func = g_Hooks.LevelRendererPlayer__renderNameTagsHook->GetFastcall<void, __int64, __int64, TextHolder*, __int64>();
	static auto nameTagsMod = moduleMgr->getModule<NameTags>();

	if (nameTagsMod->isEnabled() && nameTagsMod->nameTags.size() > 0) {
		std::string text = Utils::sanitize(a3->getText());
		std::size_t found = text.find('\n');

		if (found != std::string::npos)
			text = text.substr(0, found);

		if (nameTagsMod->nameTags.find(text) != nameTagsMod->nameTags.end())
			return;
	}

	return func(a1, a2, a3, a4);
}
