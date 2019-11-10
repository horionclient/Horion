#include "Hooks.h"

Hooks    g_Hooks;
bool isTicked = false;
//#define TEST_DEBUG

void Hooks::Init()
{
	logF("Setting up Hooks...");

	// Vtables
	{
		// GameMode::vtable
		{
			uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 01 33 D2 48 C7 41 ??");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** gameModeVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (gameModeVtable == 0x0 || sigOffset == 0x0)
				logF("C_GameMode signature not working!!!");
			else {
				g_Hooks.GameMode_tickHook = std::make_unique<FuncHook>(gameModeVtable[9], Hooks::GameMode_tick);

				g_Hooks.GameMode_startDestroyBlockHook = std::make_unique<FuncHook>(gameModeVtable[1], Hooks::GameMode_startDestroyBlock);

				g_Hooks.GameMode_getPickRangeHook = std::make_unique<FuncHook>(gameModeVtable[10], Hooks::GameMode_getPickRange);
			}
		}

		// BlockLegacy::vtable
		{
			uintptr_t sigOffset = Utils::FindSignature("48 8D ?? ?? ?? ?? ?? 48 89 ?? 4C 39"); // BlockLegacy constructor
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** blockLegacyVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (blockLegacyVtable == 0x0 || sigOffset == 0x0)
				logF("C_BlockLegacy signature not working!!!");
			else {
				g_Hooks.BlockLegacy_getRenderLayerHook = std::make_unique<FuncHook>(blockLegacyVtable[117], Hooks::BlockLegacy_getRenderLayer);

				g_Hooks.BlockLegacy_getLightEmissionHook = std::make_unique<FuncHook>(blockLegacyVtable[14], Hooks::BlockLegacy_getLightEmission);
			}
		}

		// LocalPlayer::vtable
		{
			uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 ?? ?? 49 8D ?? ?? ?? ?? ?? ?? 4D 8B");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** localPlayerVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (localPlayerVtable == 0x0 || sigOffset == 0x0)
				logF("C_LocalPlayer signature not working!!!");
			else {
				g_Hooks.Actor_isInWaterHook = std::make_unique<FuncHook>(localPlayerVtable[61], Hooks::Actor_isInWater);

				g_Hooks.Actor_startSwimmingHook = std::make_unique<FuncHook>(localPlayerVtable[181], Hooks::Actor_startSwimming);

				g_Hooks.Actor_ladderUpHook = std::make_unique<FuncHook>(localPlayerVtable[321], Hooks::Actor_ladderUp);
			}
		}

		// MoveInputHandler::vtable
		{
			uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 03 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 42 ?? 48 85 C0 74 04");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** moveInputVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (moveInputVtable == 0x0 || sigOffset == 0x0)
				logF("C_GameMode signature not working!!!");
			else {
				g_Hooks.MoveInputHandler_tickHook = std::make_unique<FuncHook>(moveInputVtable[1], Hooks::MoveInputHandler_tick);
			}
		}
	}
	
	// Signatures
	{
		void* surv_tick = reinterpret_cast<void*>(Utils::FindSignature("48 8B C4 55 48 8D 68 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 10 48 89 70 18 48 89 78 20 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 48 8B F9 8B 41 ??"));
		g_Hooks.SurvivalMode_tickHook = std::make_unique<FuncHook>(surv_tick, Hooks::SurvivalMode_tick);

		void* _sendChatMessage = reinterpret_cast<void*>(Utils::FindSignature("40 57 48 83 EC ?? 48 C7 44 24 ?? FE FF FF FF 48 89 9C 24 ?? ?? 00 00 48 8B D9 48 83 B9"));
		g_Hooks.ChatScreenController_sendChatMessageHook = std::make_unique<FuncHook>(_sendChatMessage, Hooks::ChatScreenController_sendChatMessage);
		
		void* _renderText = reinterpret_cast<void*>(Utils::FindSignature("48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 ?? 0F 29 70 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 48 89 54 24"));
		g_Hooks.RenderTextHook = std::make_unique<FuncHook>(_renderText, Hooks::RenderText);
		
		void* setupRender = reinterpret_cast<void*>(Utils::FindSignature("40 57 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B DA 48 8B F9 33 D2 ?? ?? ?? ?? ?? ?? 48 8D 4C 24 30 E8 ?? ?? ?? ?? 4C 8B CF 4C 8B C3 48 8B 57 ?? 48 8D 4C 24 ??"));
		g_Hooks.UIScene_setupAndRenderHook = std::make_unique<FuncHook>(setupRender, Hooks::UIScene_setupAndRender);
		
		void* render = reinterpret_cast<void*>(Utils::FindSignature("40 56 57 41 56 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B FA 48 8B D9 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 30 41 8B 04 36 39 05 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 33 C0"));
		g_Hooks.UIScene_renderHook = std::make_unique<FuncHook>(render, Hooks::UIScene_render);

		void* fogColorFunc = reinterpret_cast<void*>(Utils::FindSignature("0F 28 C2 C7 42 0C 00 00 80 3F F3"));
		g_Hooks.Dimension_getFogColorHook = std::make_unique<FuncHook>(fogColorFunc, Hooks::Dimension_getFogColor);

		void* ChestTick = reinterpret_cast<void*>(Utils::FindSignature("40 53 57 48 83 EC ?? 48 8B 41 ?? 48 8B FA 48 89 6C 24 ?? 48 8B D9 4C 89 74 24 ?? 48 85 C0 75 10 48 8D 51 ?? 48 8B CF E8 ?? ?? ?? ?? 48 89 43 ?? FF 43 ?? 48 85 C0"));
		g_Hooks.ChestBlockActor_tickHook = std::make_unique <FuncHook>(ChestTick, Hooks::ChestBlockActor_tick);

		void* lerpFunc = reinterpret_cast<void*>(Utils::FindSignature("8B 02 89 81 ?? 04 ?? ?? 8B 42 04 89 81 ?? ?? ?? ?? 8B 42 08 89 81 ?? ?? ?? ?? C3"));
		g_Hooks.Actor_lerpMotionHook = std::make_unique <FuncHook>(lerpFunc, Hooks::Actor_lerpMotion);

		void* getGameEdition = reinterpret_cast<void*>(Utils::FindSignature("8B 91 ?? ?? ?? ?? 85 D2 74 1C 83 EA 01"));
		g_Hooks.AppPlatform_getGameEditionHook = std::make_unique <FuncHook>(getGameEdition, Hooks::AppPlatform_getGameEdition);

		void* autoComplete = reinterpret_cast<void*>(Utils::FindSignature("48 8B C4 55 57 41 56 48 8D 68 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 ?? 48 89 70 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 41 8B F9"));
		g_Hooks.PleaseAutoCompleteHook = std::make_unique <FuncHook>(autoComplete, Hooks::PleaseAutoComplete);

		void* sendtoServer = reinterpret_cast<void*>(Utils::FindSignature("48 89 5C 24 08 57 48 ?? ?? ?? ?? ?? ?? 0F B6 41 ?? 48 8B FA 88 42 ?? 48 8D 54 24 ?? 48 8B 59 ?? 48 8B CB E8 ?? ?? ?? ?? 45 33 C9"));
		g_Hooks.LoopbackPacketSender_sendToServerHook = std::make_unique <FuncHook>(sendtoServer, Hooks::LoopbackPacketSender_sendToServer);

		void* getFov = reinterpret_cast<void*>(Utils::FindSignature("40 53 48 83 EC ?? 0F 29 74 24 ?? 0F 29 7C 24 ?? 44 0F 29 44 24 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ??"));
		g_Hooks.LevelRendererPlayer_getFovHook = std::make_unique<FuncHook>(getFov, Hooks::LevelRendererPlayer_getFov);

		void* tick_entityList = reinterpret_cast<void*>(Utils::FindSignature("48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? E8 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B D8 ?? ?? ?? ?? ?? ?? 48 99"));
		g_Hooks.MultiLevelPlayer_tickHook = std::make_unique<FuncHook>(tick_entityList, Hooks::MultiLevelPlayer_tick);

		void* keyMouseFunc = reinterpret_cast<void*>(Utils::FindSignature("40 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 0F 29 74 24 70 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 E0 49 8B F8 48 8B F1 45 33 ED 41 8B DD 89 5D DC 49 8B C8 E8"));
		g_Hooks.HIDController_keyMouseHook = std::make_unique<FuncHook>(keyMouseFunc, Hooks::HIDController_keyMouse);

		void* renderLevel = reinterpret_cast<void*>(Utils::FindSignature("40 53 56 57 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? FE FF FF FF 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 49 8B D8 48 8B FA 48 8B F1 33 D2"));
		g_Hooks.LevelRenderer_renderLevelHook = std::make_unique<FuncHook>(renderLevel, Hooks::LevelRenderer_renderLevel);

		void* clickHook = reinterpret_cast<void*>(Utils::FindSignature("48 8B C4 48 89 58 ?? 48 89 68 ?? 48 89 70 ?? 57 41 54 41 55 41 56 41 57 48 83 EC 60 44 ?? ?? ?? ?? ?? ?? ?? ?? 33 F6"));
		g_Hooks.ClickFuncHook = std::make_unique<FuncHook>(clickHook, Hooks::ClickFunc);

		void* chestScreenControllerTick = reinterpret_cast<void*>(Utils::FindSignature("48 89 5C 24 08 57 48 83 EC 20 48 8B F9 E8 ?? ?? ?? ?? 48 8B 17 48 8B CF 8B D8 FF 92 ?? ?? ?? ?? 84 C0 74 31"));
		g_Hooks.ChestScreenController_tickHook = std::make_unique<FuncHook>(chestScreenControllerTick, Hooks::ChestScreenController_tick);

		void* fullbright = reinterpret_cast<void*>(Utils::FindSignature("40 57 48 83 EC 40 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 89 74 24 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 33 C0 48 89 44 24 ?? 48 89 44 24 ?? 48 8B 01 48 8D 54 24 ??"));
		g_Hooks.GetGammaHook = std::make_unique<FuncHook>(fullbright, Hooks::GetGamma);

		void* jump = reinterpret_cast<void*>(Utils::FindSignature("40 57 48 83 EC 40 48 8B 01 48 8B F9 FF 50 ?? 8B 08 89 ?? ?? ?? ?? ?? 8B 48 ?? 89"));
		g_Hooks.JumpPowerHook = std::make_unique<FuncHook>(jump, Hooks::JumpPower);

		void* onAppSuspended = reinterpret_cast<void*>(Utils::FindSignature("48 8B C4 57 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 58 ?? 48 89 68 ?? 48 89 70 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B F1 ?? ?? ?? ?? ?? ?? ?? 48 85 C9"));
		g_Hooks.MinecraftGame_onAppSuspendedHook = std::make_unique<FuncHook>(onAppSuspended, Hooks::MinecraftGame_onAppSuspended);

		void* RakNetInstance__tick = reinterpret_cast<void*>(Utils::FindSignature("48 8B C4 55 41 56 41 57 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 89 58 ?? 48 89 70 ?? 48 89 78 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? 48 8B F1 80 B9 ?? ?? ?? ?? ?? 74 51 C6 81 ?? ?? ?? ?? ?? 66 ?? ?? ?? ?? ?? ?? ?? 74 40 48 8D 45"));
		g_Hooks.RakNetInstance_tickHook = std::make_unique<FuncHook>(RakNetInstance__tick, Hooks::RakNetInstance_tick);

#ifdef TEST_DEBUG
		void* addAction = reinterpret_cast<void*>(Utils::FindSignature("40 55 56 57 41 56 41 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 8B EA 4C 8B F1 4C 8B C2 48 8B 51 ?? 48 8B 49 ?? E8"));
		g_Hooks.InventoryTransactionManager__addActionHook = std::make_unique<FuncHook>(addAction, Hooks::InventoryTransactionManager__addAction);
#endif
	}
	MH_EnableHook(MH_ALL_HOOKS);
}

void Hooks::Restore()
{
	g_Hooks.GameMode_tickHook->Restore();
	g_Hooks.ChatScreenController_sendChatMessageHook->Restore();
	g_Hooks.RenderTextHook->Restore();
	g_Hooks.AppPlatform_getGameEditionHook->Restore();
	g_Hooks.PleaseAutoCompleteHook->Restore();
	g_Hooks.LevelRendererPlayer_getFovHook->Restore();
	g_Hooks.ChestBlockActor_tickHook->Restore();
	g_Hooks.LoopbackPacketSender_sendToServerHook->Restore();
	g_Hooks.MultiLevelPlayer_tickHook->Restore();
	g_Hooks.GameMode_startDestroyBlockHook->Restore();
	g_Hooks.HIDController_keyMouseHook->Restore();
	g_Hooks.BlockLegacy_getRenderLayerHook->Restore();
	g_Hooks.LevelRenderer_renderLevelHook->Restore();
	g_Hooks.BlockLegacy_getLightEmissionHook->Restore();
	g_Hooks.ClickFuncHook->Restore();
	g_Hooks.MoveInputHandler_tickHook->Restore();
	g_Hooks.ChestScreenController_tickHook->Restore();
	g_Hooks.GetGammaHook->Restore();
	g_Hooks.Actor_isInWaterHook->Restore();
	g_Hooks.JumpPowerHook->Restore();
	g_Hooks.MinecraftGame_onAppSuspendedHook->Restore();
	g_Hooks.Actor_ladderUpHook->Restore();
	g_Hooks.RakNetInstance_tickHook->Restore();
	g_Hooks.GameMode_getPickRangeHook->Restore();
	g_Hooks.InventoryTransactionManager_addActionHook->Restore();
}

void Hooks::GameMode_tick(C_GameMode* _this)
{
	static auto oTick = g_Hooks.GameMode_tickHook->GetFastcall<void, C_GameMode*>();
	oTick(_this);

	GameData::updateGameData(_this);
	if (_this->player == g_Data.getLocalPlayer()) {
		moduleMgr->onTick(_this);
	}
}

void Hooks::SurvivalMode_tick(C_GameMode* _this)
{
	static auto oTick = g_Hooks.SurvivalMode_tickHook->GetFastcall<void, C_GameMode*>();
	oTick(_this);
	GameData::updateGameData(_this);
	if (_this->player == g_Data.getLocalPlayer()) {
		moduleMgr->onTick(_this);
	}
}

void Hooks::ChatScreenController_sendChatMessage(uint8_t* _this)
{
	static auto oSendMessage = g_Hooks.ChatScreenController_sendChatMessageHook->GetFastcall<void, void*>();

	using addCommandToChatHistory_t = void(__fastcall*)(__int64*, char*);
	static addCommandToChatHistory_t addCommandToChatHistory = reinterpret_cast<addCommandToChatHistory_t>(Utils::FindSignature("48 89 5C 24 ?? 57 48 83 EC ?? 48 83 79 ?? ?? 48 8B FA 48 8B D9 76 46 48 8B 41 ?? 48 89 74 24 ?? 33 F6"));

	uintptr_t* textLength = reinterpret_cast<uintptr_t*>(_this + 0x710);
	if (*textLength) {
		char* message = reinterpret_cast<char*>(_this + 0x700);
		if (*reinterpret_cast<__int64*>(_this + 0x718) >= 0x10)
			message = *reinterpret_cast<char**>(message);

		if (*message == cmdMgr->prefix) {
			cmdMgr->execute(message);

			__int64* a1 = (__int64*)(*(__int64(__cdecl**)(__int64))(**(__int64**)(*(__int64*)(_this + 0x668) + 0x30i64) + 0x950i64))(*(__int64*)(*(__int64*)(_this + 0x668) + 0x30i64));
			addCommandToChatHistory(a1, (char*)(_this + 0x700)); // This will put the command in the chat history (Arrow up/down)

			__int64 v17 = 0;
			__int64* v15 = *(__int64**)(*(__int64*)(_this + 0x668) + 0x30i64);
			__int64 v16 = *v15;

			if (*(BYTE*)(_this + 0x72A))
				v17 = (*(__int64(__cdecl**)(__int64*))(v16 + 0x958))(v15);
			else
				v17 = (*(__int64(__cdecl**)(__int64*))(v16 + 0x950))(v15);
			*(DWORD*)(_this + 0x724) = *(DWORD*)(v17 + 0x20);

			*reinterpret_cast<__int64*>(_this + 0x710) = 0i64;
			*message = 0x0; // Remove command in textbox
			*textLength = 0x0; // text length
			return;
		}
	}
	oSendMessage(_this);
}

__int64 Hooks::UIScene_setupAndRender(C_UIScene* uiscene, __int64 screencontext)
{
	static auto oSetup = g_Hooks.UIScene_setupAndRenderHook->GetFastcall<__int64, C_UIScene*, __int64>();

	g_Hooks.shouldRender = uiscene->isPlayScreen();

	return oSetup(uiscene, screencontext);
}

__int64 Hooks::UIScene_render(C_UIScene* uiscene, __int64 screencontext)
{
	static auto oRender = g_Hooks.UIScene_renderHook->GetFastcall<__int64, C_UIScene*, __int64>();

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

__int64 Hooks::RenderText(__int64 a1, C_MinecraftUIRenderContext* renderCtx)
{
	static auto oText = g_Hooks.RenderTextHook->GetFastcall<__int64, __int64, C_MinecraftUIRenderContext*>();
	C_GuiData* dat = g_Data.getClientInstance()->getGuiData();
	DrawUtils::setCtx(renderCtx, dat);
	if (GameData::shouldHide())
		return oText(a1, renderCtx);

	// Call PreRender() functions
	moduleMgr->onPreRender();
	DrawUtils::flush();

	__int64 retval = oText(a1, renderCtx);

#ifdef PERFORMANCE_TEST
	std::chrono::steady_clock::time_point beginPostRender = std::chrono::steady_clock::now();
#endif

	bool shouldRenderArrayList = true;
	bool shouldRenderTabGui = true;
	bool shouldRenderCoords = false;

	// Call PostRender() functions
	{
		moduleMgr->onPostRender();
		static HudModule* hud = moduleMgr->getModule<HudModule>();
		if (hud == nullptr)
			hud = moduleMgr->getModule<HudModule>();
		else {
			shouldRenderTabGui = hud->tabgui && hud->isEnabled();
			shouldRenderArrayList = hud->arraylist && hud->isEnabled();
			shouldRenderCoords = hud->coordinates && hud->isEnabled();
		}

		static IModule* ClickGuiModule = moduleMgr->getModule<ClickGuiMod>();
		if (ClickGuiModule == nullptr)
			ClickGuiModule = moduleMgr->getModule<ClickGuiMod>();
		else if (ClickGuiModule->isEnabled()) {
			ClickGui::render();
			shouldRenderArrayList = false;
			shouldRenderCoords = false;
		}

		if (shouldRenderTabGui) TabGui::render();
	}

	{
		// Display ArrayList on the Right?
		static constexpr bool isOnRightSide = true;
		static float rcolors[4]; // Rainbow color array RGBA
		static float disabledRcolors[4]; // Rainbow Colors, but for disabled modules
		static float currColor[4]; // ArrayList collors
		static std::string horionStr = std::string("Horion");					 // Static Horion logo / text
		static float       horionStrWidth = DrawUtils::getTextWidth(&horionStr); // Graphical Width of Horion logo / text
		static std::string dlStr = std::string("discord.gg/horion");
		static float       dlStrWidth = DrawUtils::getTextWidth(&dlStr);

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
		static HudModule* hud = moduleMgr->getModule<HudModule>();
		if (hud == nullptr)
			hud = moduleMgr->getModule<HudModule>();
		else if (hud->watermark && hud->isEnabled()) {
		DrawUtils::drawText(vec2_t(windowSize.x - horionStrWidth * 1.5f - 2.f, windowSize.y - 22.f), &horionStr, nullptr, 1.5f);
		DrawUtils::drawText(vec2_t(windowSize.x - dlStrWidth * 0.85f - 2.f, windowSize.y - 10.75f), &dlStr, nullptr, 0.85f);
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
						char text[50];
						sprintf_s(text, 50, "%s [%s]", moduleNameChr, Utils::getKeybindName(keybind));
						moduleName = text;
					}

					this->textWidth = DrawUtils::getTextWidth(&moduleName);
				}

				bool operator<(const IModuleContainer& other) const {

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
					if (extendedArraylist || (*it)->isEnabled()) {
						HudModule* hud = moduleMgr->getModule<HudModule>();
						if ((*it) != hud) modContainerList.emplace(IModuleContainer(*it));
					}
				}
			}

			int a = 0;
			int b = 0;
			int c = 0;

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
				// Here comes some premium code (thank you lemon for explaining a bit)
				c++;
				b++;
				if (b < 20) a = moduleMgr->getEnabledModuleCount() * 2;
				else b = 0;
				{
					currColor[0] = rcolors[0];
					currColor[1] = rcolors[1];
					currColor[2] = rcolors[2];
					currColor[3] = rcolors[3];
				}
				Utils::ColorConvertRGBtoHSV(currColor[0], currColor[1], currColor[2], currColor[0], currColor[1], currColor[2]);
				currColor[0] += 1.f / a * c;
				Utils::ColorConvertHSVtoRGB(currColor[0], currColor[1], currColor[2], currColor[0], currColor[1], currColor[2]);

				DrawUtils::drawText(textPos, &textStr, new MC_Color(currColor), textSize);
				if (!GameData::canUseMoveKeys() && rectPos.contains(&mousePos)) {

					if (leftMouseDown) {
						DrawUtils::fillRectangle(rectPos, MC_Color(0.4f, 0.9f, 0.4f, 0.1f), it->enabled ? 0.6f : 0.6f);
						if (executeClick)
							it->backingModule->toggle();
					}
					else
						DrawUtils::fillRectangle(rectPos, MC_Color(0.3f, 0.7f, 0.3f, 0.1f), it->enabled ? 0.4f : 0.15f);
				}

				yOffset += textHeight + (textPadding * 2);
			}
			c = 0;
			modContainerList.clear();
		}

		// Draw coordinates
		if (moduleMgr->isInitialized() && shouldRenderCoords && g_Data.getLocalPlayer() != nullptr) {
			vec3_t* pos = g_Data.getLocalPlayer()->getPos();
			std::string coords = "XYZ: " + std::to_string((int)pos->x) + " / " + std::to_string((int)pos->y) + " / " + std::to_string((int)pos->z);
			DrawUtils::drawText(vec2_t(5.f, shouldRenderTabGui ? windowSize.y - 12.f : 2.f), &coords, nullptr, 1.f);
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

float* Hooks::Dimension_getFogColor(__int64 _this, float* color, float brightness)
{
	static auto oGetFogColor = g_Hooks.Dimension_getFogColorHook->GetFastcall<float*, __int64, float*, float>();

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

void Hooks::ChestBlockActor_tick(C_ChestBlockActor* _this, void* a)
{
	static auto oTick = g_Hooks.ChestBlockActor_tickHook->GetFastcall<void, C_ChestBlockActor*, void*>();
	oTick(_this, a);
	GameData::addChestToList(_this);
}

void Hooks::Actor_lerpMotion(C_Entity* _this, vec3_t motVec)
{
	static auto oLerp = g_Hooks.Actor_lerpMotionHook->GetFastcall<void, C_Entity*, vec3_t>();

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
			if (mod->xModifier == 0 && mod->yModifier == 0)
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

int Hooks::AppPlatform_getGameEdition(__int64 _this)
{
	static auto oGetEditon = g_Hooks.AppPlatform_getGameEditionHook->GetFastcall<signed int, __int64>();

	static EditionFaker* mod = moduleMgr->getModule<EditionFaker>();
	if (mod == nullptr)
		mod = moduleMgr->getModule<EditionFaker>();
	else if (mod->isEnabled()) {
		return mod->getFakedEditon();
	}

	return oGetEditon(_this);
}

void Hooks::PleaseAutoComplete(__int64 a1, __int64 a2, TextHolder* text, int a4)
{
	static auto oAutoComplete = g_Hooks.PleaseAutoCompleteHook->GetFastcall<void, __int64, __int64, TextHolder*, int>();
	char* tx = text->getText();
	if (tx != nullptr && text->getTextLength() >= 1 && tx[0] == '.') {
		std::string search = tx + 1; // Dont include the '.'
		std::transform(search.begin(), search.end(), search.begin(), ::tolower); // make the search text lowercase

		struct LilPlump {
			std::string cmdAlias;
			IMCCommand* command = 0;
			bool shouldReplace = true;

			bool operator<(const LilPlump& o) const {
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
				using syncShit_t = void(__fastcall*)(__int64*, TextHolder*);
				static syncShit_t syncShit = nullptr;
				static __int64* winrt_ptr;
				if (syncShit == nullptr) {
					uintptr_t sigOffset = Utils::FindSignature("48 8B 0D ?? ?? ?? ?? 48 8B 01 49 8B D6 FF 90 ?? ?? ?? ??");
					int offset = *reinterpret_cast<int*>(sigOffset + 3);
					winrt_ptr = *reinterpret_cast<__int64**>(sigOffset + offset + 7);
					syncShit = reinterpret_cast<syncShit_t>(*reinterpret_cast<__int64*>(*winrt_ptr + 0x460));
				}

				syncShit(winrt_ptr, text);
			}
		}

		return;
	}
	oAutoComplete(a1, a2, text, a4);
}

void Hooks::LoopbackPacketSender_sendToServer(C_LoopbackPacketSender* a, C_Packet* packet)
{
	static auto oFunc = g_Hooks.LoopbackPacketSender_sendToServerHook->GetFastcall<void, C_LoopbackPacketSender*, C_Packet*>();

	static IModule* FreecamMod = moduleMgr->getModule<Freecam>();
	static NoFall* NoFallMod = moduleMgr->getModule<NoFall>();
	static Blink* BlinkMod = moduleMgr->getModule<Blink>();
	static NoPacket* No_Packet = moduleMgr->getModule<NoPacket>();
	static Criticals* CriticalsMod = moduleMgr->getModule<Criticals>();

	if (FreecamMod == nullptr || BlinkMod == nullptr || No_Packet == nullptr) {
		FreecamMod = moduleMgr->getModule<Freecam>();
		BlinkMod = moduleMgr->getModule<Blink>();
		No_Packet = moduleMgr->getModule<NoPacket>();
	}
	else if (No_Packet->isEnabled()) {
		return;
	}
	else if (FreecamMod->isEnabled() || BlinkMod->isEnabled()) {

		C_MovePlayerPacket movePacket = C_MovePlayerPacket();
		if (movePacket.vTable == packet->vTable)
		{
			if (BlinkMod->isEnabled())
			{
				C_MovePlayerPacket* meme = reinterpret_cast<C_MovePlayerPacket*>(packet);
				meme->onGround = true; //Don't take Fall Damages when turned off
				BlinkMod->PacketMeme.push_back(new C_MovePlayerPacket(*meme)); // Saving the packets
			}
			return; // Dont call LoopbackPacketSender_sendToServer
		}
	}
	else if (!BlinkMod->isEnabled() && BlinkMod->PacketMeme.size() > 0) {

		for (std::vector<C_MovePlayerPacket*>::iterator it = BlinkMod->PacketMeme.begin(); it != BlinkMod->PacketMeme.end(); ++it)
		{
			oFunc(a, (*it));
			delete* it;
			*it = nullptr;
		}
		BlinkMod->PacketMeme.clear();
		return;
	}

	if (NoFallMod == nullptr)
		NoFallMod = moduleMgr->getModule<NoFall>();
	else if (NoFallMod->isEnabled()) {
		C_MovePlayerPacket frenchBoy = C_MovePlayerPacket();
		C_ActorFallPacket fall = C_ActorFallPacket();
		if (frenchBoy.vTable == packet->vTable && g_Data.getLocalPlayer() != nullptr && !g_Data.getLocalPlayer()->onGround
			&& g_Data.getLocalPlayer()->fallDistance > 2.5f) {
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
	else if (CriticalsMod->isEnabled()) {
		C_MovePlayerPacket frenchBoy = C_MovePlayerPacket();
		if (frenchBoy.vTable == packet->vTable && g_Data.getLocalPlayer() != nullptr 
			&& g_Data.getLocalPlayer()->fallDistance == 0.f)
		{
			C_MovePlayerPacket* p = reinterpret_cast<C_MovePlayerPacket*>(packet);
			p->onGround = false;
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
	static auto oGetFov = g_Hooks.LevelRendererPlayer_getFovHook->GetFastcall<float, __int64, float, bool>();
	static void* renderItemInHand = reinterpret_cast<void*>(Utils::FindSignature("0F 28 F0 F3 44 0F 10 3D ?? ?? ?? ?? F3 41 0F 59 F7"));
	static void* setupCamera = reinterpret_cast<void*>(Utils::FindSignature("44 0F 28 D8 F3 44 0F 59 1D ?? ?? ?? ?? 41 0F B6 4E ??"));

	if (_ReturnAddress() == renderItemInHand) {
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

void Hooks::MultiLevelPlayer_tick(C_EntityList* _this)
{
	static auto oTick = g_Hooks.MultiLevelPlayer_tickHook->GetFastcall<void, C_EntityList*>();
	oTick(_this);
	GameData::EntityList_tick(_this);
}

void Hooks::GameMode_startDestroyBlock(C_GameMode* _this, vec3_ti* a2, uint8_t face, void* a4, void* a5)
{
	static auto oFunc = g_Hooks.GameMode_startDestroyBlockHook->GetFastcall<void, C_GameMode*, vec3_ti*, uint8_t, void*, void*>();

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
							if (id != 0 && (!isVeinMiner || (id == selectedBlockId && data == selectedBlockData)))
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

void Hooks::HIDController_keyMouse(C_HIDController* _this, void* a2, void* a3)
{
	static auto oFunc = g_Hooks.HIDController_keyMouseHook->GetFastcall<void, C_HIDController*, void*, void*>();
	GameData::setHIDController(_this);
	isTicked = true;
	oFunc(_this, a2, a3);
	return;
}

int Hooks::BlockLegacy_getRenderLayer(C_BlockLegacy* a1)
{
	static auto oFunc = g_Hooks.BlockLegacy_getRenderLayerHook->GetFastcall<int, C_BlockLegacy*>();

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

__int8* Hooks::BlockLegacy_getLightEmission(C_BlockLegacy* a1, __int8* a2)
{
	static auto oFunc = g_Hooks.BlockLegacy_getLightEmissionHook->GetFastcall<__int8*, C_BlockLegacy*, __int8*>();

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
	static auto oFunc = g_Hooks.LevelRenderer_renderLevelHook->GetFastcall<__int64, __int64, __int64, __int64>();

	using reloadShit_t = void(__fastcall*)(__int64);
	static reloadShit_t reloadChunk = reinterpret_cast<reloadShit_t>(Utils::FindSignature("48 8B C4 56 57 41 54 41 56 41 57 48 83 EC ?? 48 C7 40 ?? FE FF FF FF 48 89 58 ?? 48 89 68 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B F9 4C"));

	static IModule* xray = moduleMgr->getModule<Xray>();
	if (xray == nullptr) {
		xray = moduleMgr->getModule<Xray>();
	}
	else {
		static bool lastState = false;
		if (lastState != xray->isEnabled()) {
			lastState = xray->isEnabled();
			unsigned long long* v5; // rdi
			unsigned long long* i; // rbx

			v5 = *(unsigned long long**)(_this + 32);
			for (i = (unsigned long long*) * v5; i != v5; i = (unsigned long long*) * i)
				reloadChunk(i[3]);
		}
	}

	return oFunc(_this, a2, a3);
}

void Hooks::ClickFunc(__int64 a1, char a2, char a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7, char a8) {

	static auto oFunc = g_Hooks.ClickFuncHook->GetFastcall<void, __int64, char, char, __int16, __int16, __int16, __int16, char>();
	static IModule* clickGuiModule = moduleMgr->getModule<ClickGuiMod>();

	if (clickGuiModule == nullptr)
		clickGuiModule = moduleMgr->getModule<ClickGuiMod>();
	else if (clickGuiModule->isEnabled()) {
		if (isTicked) {
			isTicked = false;
			return;
		}
	}
	oFunc(a1, a2, a3, a4, a5, a6, a7, a8);
}

__int64 Hooks::MoveInputHandler_tick(C_MoveInputHandler* a1, C_Entity* a2)
{
	static auto oTick = g_Hooks.MoveInputHandler_tickHook->GetFastcall<__int64, C_MoveInputHandler*, C_Entity*>();
	return oTick(a1, a2);
}

__int64 Hooks::ChestScreenController_tick(C_ChestScreenController* a1)
{
	static auto oFunc = g_Hooks.ChestScreenController_tickHook->GetFastcall<__int64, C_ChestScreenController*>();

	static ChestStealer* ChestStealerMod = moduleMgr->getModule<ChestStealer>();
	if (ChestStealerMod == nullptr)
		ChestStealerMod = moduleMgr->getModule<ChestStealer>();
	else {
		ChestStealerMod->chestScreenController = a1;
	}

	return oFunc(a1);
}

__int64 Hooks::GetGamma(__int64 a1)
{
	static auto oFunc = g_Hooks.GetGammaHook->GetFastcall<__int64, __int64>();

	static FullBright* fullBrightModule = moduleMgr->getModule<FullBright>();
	if (fullBrightModule == nullptr)
		fullBrightModule = moduleMgr->getModule<FullBright>();

	static __int64 v7 = 0;
	if (v7 == 0) {
		__int64 v6 = oFunc(a1); // Calls to ClientInstance, returns options ptr
		if (*(bool*)(v6 + 0x1001))
			v7 = *(__int64*)(v6 + 0x7D8);
		else
			v7 = *(__int64*)(v6 + 0x138);
	}
	else {
		if (fullBrightModule != nullptr)
			fullBrightModule->gammaPtr = reinterpret_cast<float*>(v7 + 0xF0);
	}

	return oFunc(a1);
}

bool Hooks::Actor_isInWater(C_Entity* _this)
{
	static auto oFunc = g_Hooks.Actor_isInWaterHook->GetFastcall<bool, C_Entity*>();

	if (g_Data.getLocalPlayer() != _this)
		return oFunc(_this);

	static AirSwim* AirSwimModule = moduleMgr->getModule<AirSwim>();
	if (AirSwimModule == nullptr)
		AirSwimModule = moduleMgr->getModule<AirSwim>();
	else if (AirSwimModule->isEnabled())
		return true;

	return oFunc(_this);
}

void Hooks::JumpPower(C_Entity* a1, float a2)
{
	static auto oFunc = g_Hooks.JumpPowerHook->GetFastcall<void, C_Entity*, float>();
	static HighJump* HighJumpMod = moduleMgr->getModule<HighJump>();
	if (HighJumpMod == nullptr)
		HighJumpMod = moduleMgr->getModule<HighJump>();
	else if (HighJumpMod->isEnabled() && g_Data.getLocalPlayer() == a1) {
		a1->velocity.y = HighJumpMod->jumpPower;
		return;
	}
	oFunc(a1, a2);
}

__int64 Hooks::MinecraftGame_onAppSuspended(__int64 _this)
{
	static auto oFunc = g_Hooks.MinecraftGame_onAppSuspendedHook->GetFastcall<__int64, __int64>();
	configMgr->saveConfig();
	return oFunc(_this);
}

void Hooks::Actor_ladderUp(C_Entity* _this)
{
	static auto oFunc = g_Hooks.Actor_ladderUpHook->GetFastcall<void, C_Entity*>();

	static IModule* FastLadderModule = moduleMgr->getModule<FastLadder>();
	if (FastLadderModule == nullptr)
		FastLadderModule = moduleMgr->getModule<FastLadder>();
	else if (FastLadderModule->isEnabled() && g_Data.getLocalPlayer() == _this) {
		_this->velocity.y = 0.6f;
		return;
	}
	return oFunc(_this);

}

void Hooks::Actor_startSwimming(C_Entity* _this)
{
	static auto oFunc = g_Hooks.Actor_startSwimmingHook->GetFastcall<void, C_Entity*>();

	static IModule* JesusModule = moduleMgr->getModule<Jesus>();
	if (JesusModule == nullptr)
		JesusModule = moduleMgr->getModule<Jesus>();
	else if (JesusModule->isEnabled() && g_Data.getLocalPlayer() == _this) {
		return;
	}
	oFunc(_this);
}

void Hooks::RakNetInstance_tick(C_RakNetInstance* _this,__int64 a2,__int64 a3)
{
	static auto oTick = g_Hooks.RakNetInstance_tickHook->GetFastcall<void, C_RakNetInstance*,__int64,__int64>();
	GameData::setRakNetInstance(_this);
	oTick(_this,a2,a3);
}

float Hooks::GameMode_getPickRange(C_GameMode* _this, __int64 a2, char a3)
{
	static auto oFunc = g_Hooks.GameMode_getPickRangeHook->GetFastcall<float, C_GameMode*, __int64, char>();
	static InfiniteBlockReach* InfiniteBlockReachModule = moduleMgr->getModule<InfiniteBlockReach>();
	if (InfiniteBlockReachModule == nullptr)
		InfiniteBlockReachModule = moduleMgr->getModule<InfiniteBlockReach>();
	else if (InfiniteBlockReachModule->isEnabled())
		return InfiniteBlockReachModule->getBlockReach();
	static ClickTP* clickTP = moduleMgr->getModule<ClickTP>();
	if (clickTP == nullptr)
		clickTP = moduleMgr->getModule<ClickTP>();
	else if (clickTP->isEnabled() && !InfiniteBlockReachModule->isEnabled())
		return 255;

	return oFunc(_this, a2, a3);
}

void Hooks::InventoryTransactionManager_addAction(C_InventoryTransactionManager* a1, C_InventoryAction* a2)
{
	static auto Func = g_Hooks.InventoryTransactionManager_addActionHook->GetFastcall<void, C_InventoryTransactionManager*, C_InventoryAction*>();
	Func(a1, a2);
}
