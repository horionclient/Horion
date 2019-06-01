#include "Hooks.h"
#include "../Directx/Directx.h"

Hooks    g_Hooks;

void Hooks::Init()
{
	logF("Setting up Hooks...");
	// GameMode::tick Signature
	// CC 8B 41 ?? 89 41 ?? C3
	//    ^^ Function starts here
	void* func = reinterpret_cast<void*>(Utils::FindSignature("CC 8B 41 ?? 89 41 ?? C3") + 1);
	g_Hooks.gameMode_tickHook = std::make_unique<FuncHook>(func, Hooks::GameMode_tick);
	g_Hooks.gameMode_tickHook->init();

	// SurvivalMode::tick Sig
	// 48 8B C4 55 48 8D 68 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 10 48 89 70 18 48 89 78 20 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 48 8B F9 8B 41 ??
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


	// 
	void* _shit = reinterpret_cast<void*>(Utils::FindSignature("30 5F C3 CC 48 8B C4 55 56 57 41 54") + 4);
	g_Hooks.renderTextHook = std::make_unique<FuncHook>(_shit, Hooks::renderText);
	g_Hooks.renderTextHook->init();

	// I8n::get doesnt want to work
	//void *_shitshikt = reinterpret_cast<void*>(g_Data.getModule()->ptrBase + 0xB577C0);
	//g_Hooks.I8n_getHook = std::make_unique<FuncHook>(_shitshikt, Hooks::I8n_get);
	//g_Hooks.I8n_getHook->init();

	void* boii = reinterpret_cast<void*>(Utils::FindSignature("0F 28 C2 C7 42 0C 00 00 80 3F F3"));
	g_Hooks.Dimension_getFogColorHook = std::make_unique<FuncHook>(boii, Hooks::Dimension_getFogColor);
	g_Hooks.Dimension_getFogColorHook->init();

	void* destroyBlok = reinterpret_cast<void*>(Utils::FindSignature("55 57 41 56 48 8D 68 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 ?? 48 89 70 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 45 0F B6 F0") - 3);
	g_Hooks.GameMode_destroyBlockHook = std::make_unique <FuncHook>(destroyBlok, Hooks::GameMode_destroyBlock);
	g_Hooks.GameMode_destroyBlockHook->init();

	void* ChestTick = reinterpret_cast<void*>(Utils::FindSignature("40 53 57 48 83 EC ?? 48 8B 41 ?? 48 8B FA 48 89 6C 24 ?? 48 8B D9 4C 89 74 24 ?? 48 85 C0 75 10 48 8D 51 ?? 48 8B CF E8 ?? ?? ?? ?? 48 89 43 ?? FF 43 ?? 48 85 C0"));
	g_Hooks.ChestBlockActor_tickHook = std::make_unique <FuncHook>(ChestTick, Hooks::ChestBlockActor_tick);
	g_Hooks.ChestBlockActor_tickHook->init();

	void* lerpFunc = reinterpret_cast<void*>(Utils::FindSignature("8B 02 89 81 ?? 0F ?? ?? 8B 42 04 89 81 ?? ?? ?? ?? 8B 42 08 89 81 ?? ?? ?? ?? C3"));
	g_Hooks.Actor_lerpMotionHook = std::make_unique <FuncHook>(lerpFunc, Hooks::Actor_lerpMotion);
	g_Hooks.Actor_lerpMotionHook->init();

	void* getGameEdition = reinterpret_cast<void*>(Utils::FindSignature("8B 91 ?? ?? ?? ?? 85 D2 74 1C 83 EA 01"));
	g_Hooks.AppPlatform_getGameEditionHook = std::make_unique <FuncHook>(getGameEdition, Hooks::AppPlatform_getGameEdition);
	g_Hooks.AppPlatform_getGameEditionHook->init();

	void* autoComplete = reinterpret_cast<void*>(Utils::FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 44 89 4C 24 ?? 4D 8B F8"));
	g_Hooks.autoComplete_Hook = std::make_unique <FuncHook>(autoComplete, Hooks::pleaseAutoComplete);
	g_Hooks.autoComplete_Hook->init();

	void* sendtoServer = reinterpret_cast<void*>(Utils::FindSignature("48 89 5C 24 08 57 48 ?? ?? ?? ?? ?? ?? 0F B6 41 ?? 48 8B FA 88 42 ?? 48 8D 54 24 ?? 48 8B 59 ?? 48 8B CB E8 ?? ?? ?? ?? 48 8B D0 45 33 C9"));
	g_Hooks.sendToServerHook = std::make_unique <FuncHook>(sendtoServer, Hooks::sendToServer);
	g_Hooks.sendToServerHook->init();

	void* getFov = reinterpret_cast<void*>(Utils::FindSignature("40 53 48 83 EC ?? 0F 29 74 24 ?? 0F 29 7C 24 ?? 44 0F 29 4C 24 ?? 48 8B 05"));
	g_Hooks.levelRendererPlayer_getFovHook = std::make_unique<FuncHook>(getFov, Hooks::LevelRendererPlayer_getFov);
	g_Hooks.levelRendererPlayer_getFovHook->init();

	void* mob_isAlive = reinterpret_cast<void*>(Utils::FindSignature("48 83 EC ?? 80 B9 ?? ?? ?? ?? 00 75 ?? 48 8B 01 48 8D"));
	g_Hooks.mob_isAliveHook = std::make_unique<FuncHook>(mob_isAlive, Hooks::Mob_isAlive);
	g_Hooks.mob_isAliveHook->init();

	void* tick_entityList = reinterpret_cast<void*>(Utils::FindSignature("40 53 48 83 EC ?? 48 8B D9 E8 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 49 8B C8 4D 85 C0 75 07 ?? ?? ?? ?? ?? ?? ?? 4C 8B 49 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 49 ?? 49 2B C9 48 F7 E9 48 C1 FA 03 48 8B C2 48 C1 E8 3F 48 03 D0 83 FA 01 7E 36 49 8D 41 ?? 48 85 C0 74 2D 80 78 ?? 00 74 27 ?? ?? ?? ?? ?? ?? ?? 49 8B C0 4D 85 C0 75 03 48 8B C1 8B ?? ?? ?? ?? ?? 4D 85 C0"));
	g_Hooks.MultiLevelPlayerHook = std::make_unique<FuncHook>(tick_entityList, Hooks::MultiLevelPlayer_tick);
	g_Hooks.MultiLevelPlayerHook->init();

	/*void* CheckFallDamage = reinterpret_cast<void*>(Utils::FindSignature("48 83 EC 38 ?? ?? ?? ?? ?? ?? ?? 4D 8B D8 49 8B 0A ?? ?? ?? ?? ?? ?? 49 8B 42 ?? 48 2B C1 48 C1 F8 03 66 44 3B C0 73 39 ?? ?? ?? ?? ?? ?? ?? 48 85 C0 74 2D 80 78 ?? 08 75 27 F2 0F 10 40 ?? 8B 40 ?? 0F 28 C8 89 44 24 ?? F3 0F 10 54 24 ?? F2 0F 11 44 24 ?? F3 0F 10 5C 24 ?? 0F C6 C9 55 EB 09"));
	g_Hooks.LocalPlayer_CheckFallDamageHook = std::make_unique<FuncHook>(CheckFallDamage, Hooks::LocalPlayer_CheckFallDamage);
	g_Hooks.LocalPlayer_CheckFallDamageHook->init();*/

	void* startDestroyBlockFunc = reinterpret_cast<void*>(Utils::FindSignature("40 55 53 56 57 41 56 41 57 48 8D 6C 24 D1 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? 0F 29 ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 4D 8B F9 48 8B F2 48 8B F9 44 88 45 BF E8 ?? ?? ?? ?? 41 C6 07 00 84 C0 75 07 32 C0 E9 ?? ?? ?? ?? 48 8B 4F ?? 48 8B 01 FF 90 ?? ?? ?? ?? 84 C0"));
	g_Hooks.GameMode_startDestroyHook = std::make_unique<FuncHook>(startDestroyBlockFunc, Hooks::GameMode_startDestroyBlock);
	g_Hooks.GameMode_startDestroyHook->init();

	void* keyMouseFunc = reinterpret_cast<void*>(Utils::FindSignature("40 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 70 48 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 F0 49 8B F0 48 8B F9 45 33 ED 41 8B DD 89 5D EC 49 8B C8"));
	g_Hooks.HIDController_keyMouseHook = std::make_unique<FuncHook>(keyMouseFunc, Hooks::HIDController_keyMouse);
	g_Hooks.HIDController_keyMouseHook->init();

	//logF("Hooks hooked");
}

void Hooks::Restore()
{
	g_Hooks.gameMode_tickHook->Restore();
	g_Hooks.chatScreen_sendMessageHook->Restore();
	g_Hooks.d3d11_presentHook->Restore();
	g_Hooks.renderTextHook->Restore();
	//g_Hooks.I8n_getHook->Restore();
	g_Hooks.GameMode_destroyBlockHook->Restore();
	g_Hooks.AppPlatform_getGameEditionHook->Restore();
	g_Hooks.autoComplete_Hook->Restore();
	g_Hooks.levelRendererPlayer_getFovHook->Restore();
	g_Hooks.ChestBlockActor_tickHook->Restore();
	g_Hooks.sendToServerHook->Restore();
	g_Hooks.MultiLevelPlayerHook->Restore();
	g_Hooks.mob_isAliveHook->Restore();
	g_Hooks.LocalPlayer_CheckFallDamageHook->Restore();
	g_Hooks.GameMode_startDestroyHook->Restore();
	g_Hooks.HIDController_keyMouseHook->Restore();
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

		_this->player->getEntityTypeId();
	}
}

void Hooks::HIDController_keyMouse(void* a1, void* a2, void* a3)
{
	static auto oFunc = g_Hooks.HIDController_keyMouseHook->GetOriginal<HIDController_keyMouse_t>();
	GameData::addHIDController(a1);
	oFunc(a1,a2,a3); // Call Original Func
	
}

void Hooks::GameMode_startDestroyBlock(C_GameMode* a, vec3_ti* a2, uint8_t face, void* a4, void* a5)
{
	static auto oFunc = g_Hooks.GameMode_startDestroyHook->GetOriginal<GameMode_startDestroyBlock_t>();
	static auto oDestroyBlock = g_Hooks.GameMode_destroyBlockHook->GetOriginal<GameMode_destroyBlock_t>();

	static IModule* nukerModule = moduleMgr->getModule<Nuker>();
	static IModule* instaBreakModule = moduleMgr->getModule<InstaBreak>();
	if (nukerModule == nullptr || instaBreakModule == nullptr)
	{
		nukerModule = moduleMgr->getModule<Nuker>();
		instaBreakModule = moduleMgr->getModule<InstaBreak>();
	}
	else {
		if (nukerModule->isEnabled()) {
			vec3_ti tempPos;
			int x = 0;
			int z = 0;

			for (int x = -4; x < 4; x++) {
				for (int y = -4; y < 4; y++) {
					for (int z = -4; z < 4; z++) {
						tempPos.x = a2->x + x;
						tempPos.y = a2->y + y;
						tempPos.z = a2->z + z;
						if (tempPos.y > 0)
							oDestroyBlock(a, &tempPos, face);
					}
				}
			}
			return;
		}
		if (instaBreakModule->isEnabled()) {
			oDestroyBlock(a, a2, face);
			return;
		}
	}
	
	oFunc(a, a2, face,a4,a5);
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

void Hooks::LocalPlayer_CheckFallDamage(C_LocalPlayer* a, float* a2, void* a3)
{
	static auto oFunc = g_Hooks.LocalPlayer_CheckFallDamageHook->GetOriginal<LocalPlayer_CheckFallDamage_t>();


	static IModule* mod = moduleMgr->getModule<NoFall>();
	if (mod == nullptr)
		mod = moduleMgr->getModule<NoFall>();
	else if (mod->isEnabled()) {
		return;
	}

	oFunc(a, a2, a3);
}

void Hooks::Actor_lerpMotion(C_Entity * _this, vec3_t motVec)
{
	static auto oLerp = g_Hooks.Actor_lerpMotionHook->GetOriginal<Actor_lerpMotion_t>();

	if (g_Data.getLocalPlayer() != _this)
		return oLerp(_this, motVec);

	static IModule* mod = moduleMgr->getModule<NoKnockBack>();
	if (mod == nullptr)
		mod = moduleMgr->getModule<NoKnockBack>();
	else if (mod->isEnabled()) {
		// Do nothing i guess
		// Do some stuff with modifiers here maybe
		static void* networkSender = reinterpret_cast<void*>(Utils::FindSignature("EB ?? 4C 8B 64 24 ?? 49 8B 44 24 ?? 48 8B"));
		if (networkSender == 0x0)
			logF("Network Sender not Found!!!");
		if (networkSender == _ReturnAddress()) // Check if we are being called from the network receiver
			return;
	}

	oLerp(_this, motVec);
}

__int64 Hooks::AppPlatform_getGameEdition(__int64 _this)
{
	static auto oGetEditon = g_Hooks.AppPlatform_getGameEditionHook->GetOriginal<AppPlatform_getGameEdition_t>();

	static EditionFaker* mod = static_cast<EditionFaker*>(moduleMgr->getModule<EditionFaker>());
	if (mod == nullptr)
		mod = static_cast<EditionFaker*>(moduleMgr->getModule<EditionFaker>());
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

	static IModule* mod = moduleMgr->getModule<Freecam>();
	static IModule* mod2 = moduleMgr->getModule<NoFall>();
	static Blink* mod3 = reinterpret_cast<Blink*>(moduleMgr->getModule<Blink>());
	static NoPacket* No_Packet = reinterpret_cast<NoPacket*>(moduleMgr->getModule<NoPacket>());

	if (mod == nullptr || mod2 == nullptr || mod3 == nullptr || No_Packet == nullptr) {
		mod = moduleMgr->getModule<Freecam>();
		mod2 = moduleMgr->getModule<NoFall>();
		mod3 = reinterpret_cast<Blink*>(moduleMgr->getModule<Blink>());
		No_Packet = reinterpret_cast<NoPacket*>(moduleMgr->getModule<NoPacket>());
	}
	else if (No_Packet->isEnabled()) {
		return;
	}
	else if (mod->isEnabled() || mod3->isEnabled()) {

		C_MovePlayerPacket frenchBoy = C_MovePlayerPacket();
		if (frenchBoy.vTable == packet->vTable)
		{
			if (mod3->isEnabled())
			{
				C_MovePlayerPacket* meme = reinterpret_cast<C_MovePlayerPacket*>(packet);
				meme->onGround = false; //Don't take Fall Damages when turned off
				mod3->PacketMeme.push_back(new C_MovePlayerPacket(*meme)); // Saving the packets
			}
			return; // Dont call sendToServer
		}
	}
	else if (!mod3->isEnabled() && mod3->PacketMeme.size() > 0) {

		for (std::vector<C_MovePlayerPacket*>::iterator it = mod3->PacketMeme.begin(); it != mod3->PacketMeme.end(); ++it)
		{
			oFunc(a, (*it));
			delete *it;
			*it = nullptr;
		}
		mod3->PacketMeme.clear(); 
		return;
	}
	else if (mod2->isEnabled()) {
		C_MovePlayerPacket frenchBoy = C_MovePlayerPacket();
		if (frenchBoy.vTable == packet->vTable) {
			C_MovePlayerPacket* p = reinterpret_cast<C_MovePlayerPacket*>(packet);
			p->onGround = false;
		}
	}

	oFunc(a, packet);
}

float Hooks::LevelRendererPlayer_getFov(__int64 a1, float a2, bool a3)
{
	static auto oGetFov = g_Hooks.levelRendererPlayer_getFovHook->GetOriginal<getFov_t>();
	static void* renderItemInHand = reinterpret_cast<void*>(Utils::FindSignature("F3 44 0F 10 2D ?? ?? ?? ?? F3 41 0F 59 C5 0F 28 DE F3"));
	static void* setupCamera = reinterpret_cast<void*>(Utils::FindSignature("48 8B 8E ?? ?? ?? ?? 44 0F 28 C0"));

	if (_ReturnAddress() == renderItemInHand) {
		//static float yess = 0;
		//yess += 0.03f;
		//return 60 + 40 + sinf(yess) * 50;
		return oGetFov(a1, a2, a3);
	}
	if (_ReturnAddress() == setupCamera) {
		return oGetFov(a1, a2, a3);
	}
#ifdef _DEBUG
	__debugbreak(); // IF we reach here, a sig is broken
#endif
	return oGetFov(a1, a2, a3);
}

bool Hooks::Mob_isAlive(C_Entity* a1)
{
	static auto oIsAlive = g_Hooks.mob_isAliveHook->GetOriginal<mob_isAlive_T>();

	//if (a1 == g_Data.getLocalPlayer())
		//return true;

	return oIsAlive(a1);
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
			ICommand* command;
			bool shouldReplace = true;

			bool operator<(const LilPlump &o) const {
				return cmdAlias < o.cmdAlias;
			}
		}; // This is needed so the std::set sorts it alphabetically

		std::set<LilPlump> searchResults;

		std::vector<ICommand*>* commandList = cmdMgr->getCommandList();
		for (std::vector<ICommand*>::iterator it = commandList->begin(); it != commandList->end(); ++it) { // Loop through commands
			ICommand* c = *it;
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
				static syncShit sync = reinterpret_cast<syncShit>(Utils::FindSignature("40 53 48 83 EC ?? 8B 0D ?? ?? ?? ?? 48 8B DA FF 15 ?? ?? ?? ?? 48 85 C0 74 ?? 48 83 38 00 74 ?? E8 ?? ?? ?? ?? 48 8B D3 48 8B 08 48 8B 01"));
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

	uintptr_t* idk = reinterpret_cast<uintptr_t*>(_this + 0x688);
	if (*idk) {
		char* message = reinterpret_cast<char*>(_this + 0x678);
		if (*reinterpret_cast<__int64*>(_this + 0x690) >= 0x10)
			message = *reinterpret_cast<char**>(message);

		if (*message == '.') {
			cmdMgr->execute(message);

			__int64* i = 0;
			for (i = *reinterpret_cast<__int64**>(_this + 0x6A0); i[4] > 0x64ui64; i = *reinterpret_cast<__int64**>(_this + 0x6A0))
			{
				sub_140074FA0(*reinterpret_cast<__int64*>(i[1] + 8 * (i[3] & (i[2] - 1i64))));
				bool v15 = i[4]-- == 1i64;
				if (v15)
					i[3] = 0i64;
				else
					++i[3];
			}

			dequeuePushBack(i, reinterpret_cast<__int64>(_this + 0x678)); // This will put the command in the chat history (Arrow up/down)
			*reinterpret_cast<__int64*>(_this + 0x6A8) = *reinterpret_cast<__int64*>(*reinterpret_cast<__int64*>(_this + 0x6A0) + 0x20);

			*reinterpret_cast<__int64*>(_this + 0x688) = 0i64;
			*message = 0x0; // Remove command in textbox
			*idk = 0x0; // text length
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

__int64 __fastcall Hooks::renderText(__int64 yeet, C_MinecraftUIRenderContext* renderCtx)
{
	static auto oText = g_Hooks.renderTextHook->GetOriginal<renderText_t>();
	DrawUtils::setCtx(renderCtx, g_Data.getClientInstance()->getGuiData());

	// Call PreRender() functions
	{
		moduleMgr->onPreRender();
		TabGui::render();
		DrawUtils::flush();
	}
	
	__int64 retval = oText(yeet, renderCtx);

#ifdef PERFORMANCE_TEST
	std::chrono::steady_clock::time_point beginPostRender = std::chrono::steady_clock::now();
#endif

	// Call PostRender() functions
	{
		moduleMgr->onPostRender();
	}
	
	// Should we hide the gui? Then stop execution here
	if (GameData::ShouldHide()) {
		DrawUtils::flush();
		return retval;
	}
	
	static float rcolors[4]; // Rainbow color array RGBA
	static float disabledRcolors[4]; // Rainbow Colors, but for disabled modules
	static std::string horionStr = std::string("Horion");					 // Static Horion logo / text
	static float       horionStrWidth = DrawUtils::getTextWidth(&horionStr); // Graphical Width of Horion logo / text

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
		if (GameData::shouldOnTheRight())
			DrawUtils::drawText(vec2_t(1, 1), &horionStr, new MC_Color(rcolors));
		else
			DrawUtils::drawText(vec2_t(windowSize.x - horionStrWidth - 1, 1), &horionStr, new MC_Color(rcolors));
	}

	// Draw ArrayList
	if (moduleMgr->isInitialized()) {
		struct IModuleContainer {
			// Struct used to Sort IModules in a std::set
			IModule* backingModule;
			std::string moduleName;
			bool enabled;
			int keybind;
			float textWidth;

			IModuleContainer(IModule* mod) {
				this->moduleName = mod->getModuleName();
				this->enabled = mod->isEnabled();
				this->keybind = mod->getKeybind();
				this->backingModule = mod;
				
				char yikes[50];
				sprintf_s(yikes, 50, "%s [%s]", moduleName.c_str(), Utils::getKeybindName(keybind));
				moduleName = std::string(yikes);

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
				return this->textWidth < other.textWidth;
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
		const bool extendedArraylist = g_Data.getLocalPlayer() == nullptr ? /* not ingame */ true : /* ingame */(GameData::canUseMoveKeys() ? true : true);
		// Display ArrayList on the Right?
		const bool isOnRightSide = GameData::shouldOnTheRight();
		std::set<IModuleContainer> modContainerList;
		// Fill modContainerList with Modules
		{
			std::vector<IModule*>* moduleList = moduleMgr->getModuleList();

			for (std::vector<IModule*>::iterator it = moduleList->begin(); it != moduleList->end(); ++it) {
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
				}else
					DrawUtils::fillRectangle(rectPos, MC_Color(0.3f, 0.7f, 0.3f, 0.1f), it->enabled ? 0.4f : 0.15f);
			}else
				DrawUtils::fillRectangle(rectPos, MC_Color(0.f, 0.1f, 0.1f, 0.1f), it->enabled ? 0.4f : 0.15f);
			
			yOffset += textHeight + (textPadding * 2);
		}
		modContainerList.clear();
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

float * Hooks::Dimension_getFogColor(__int64 a1, float * color, float brightness)
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
	return oGetFogColor(a1, color, brightness);


}

void Hooks::GameMode_destroyBlock(void * _this, vec3_ti * pos, uint8_t face)
{
	static auto oDestroyBlock = g_Hooks.GameMode_destroyBlockHook->GetOriginal<GameMode_destroyBlock_t>();

	oDestroyBlock(_this, pos, face);
}