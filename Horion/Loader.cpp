#include "Loader.h"


_Offsets Offsets = _Offsets();

int bKillAura;
SlimUtils::SlimMem mem;
const SlimUtils::SlimModule* gameModule;
bool isRunning = true;

#if defined _M_X64
#pragma comment(lib, "MinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "MinHook.x86.lib")
#endif

DWORD WINAPI keyThread(LPVOID lpParam)
{
	logF("Key thread started");
	C_ClientInstance* clientInstance = g_Data.getClientInstance();
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	C_GameMode* gameMode = g_Data.getCGameMode();

	bool* keyMap = static_cast<bool*>(malloc(0xFF * 4 + 0x4));
	if(keyMap == 0)
		throw std::exception("Keymap 1 not located");

	bool* keyMapAddr = 0x0;
	uintptr_t sigOffset = Utils::FindSignature("48 8D 0D ?? ?? ?? ?? 89 3C 81 E9");
	if (sigOffset != 0x0) {
		int offset = *reinterpret_cast<int*>((sigOffset + 3)); // Get Offset from code
		keyMapAddr = reinterpret_cast<bool*>(sigOffset + offset + /*length of instruction*/ 7); // Offset is relative
	}
	else {
		logF("!!!KeyMap not located!!!");
		throw std::exception("Keymap not located");
	}

	while (isRunning) {
		if (GameData::isKeyDown('L') && GameData::isKeyDown(VK_CONTROL) || GameData::shouldTerminate()) { // Press L to uninject
			isRunning = false;
			
			break;
		}
		/*

		if (GameData::isKeyPressed('B')) {
			clientInstance->grabMouse();
		}

		if (GameData::isKeyPressed('N')) {
			clientInstance->releaseMouse();
		}

		if (GameData::isKeyPressed('O')) {
			
			static uintptr_t screenModelBase = 0x0;
			if (screenModelBase == 0x0) {
				uintptr_t sigOffset = Utils::FindSignature("41 89 86 ?? ?? ?? ?? 48 8B 4C 24 ?? 48 89 0D ?? ?? ?? ?? 48 8B 4C 24 ?? 48 89 0D");
				if (sigOffset != 0x0) {
					int offset = *reinterpret_cast<int*>((sigOffset + 15)); // Get Offset from code
					screenModelBase = sigOffset + offset + /*length of instruction/ 7 + 12; // Offset is relative
				}
				else
					logF("screenModelBase not found!!!");
			}
			uintptr_t* rcx = reinterpret_cast<uintptr_t*>(mem.ReadPtr<uintptr_t>(screenModelBase, { 0, 0x60, 0x10, 0x4B8, 0x0, 0xA8, 0x58, 0x5E0 }) + 0x10); //1.11.0

			C_ClientInstanceScreenModel* cli = reinterpret_cast<C_ClientInstanceScreenModel*>(rcx);
			cli->sendChatMessage("      /\\");
			Sleep(2300);
			cli->sendChatMessage("     /  \\");
			Sleep(2300);
			cli->sendChatMessage("    /    \\");
			Sleep(2300);
			cli->sendChatMessage("   /   @  \\");
			Sleep(2300);
			cli->sendChatMessage("  /_______\\");
		}
		
		if (GameData::isKeyPressed('J') && localPlayer != 0x0) {
			/*if (localPlayer != 0x0) {
				C_MovePlayerPacket* Packet = new C_MovePlayerPacket();
			
				Packet->entityRuntimeID = localPlayer->entityRuntimeId;
				Packet->Position = localPlayer->eyePos0;
				Packet->pitch = localPlayer->pitch;
				Packet->yaw = localPlayer->yaw;
				Packet->headYaw = localPlayer->yaw;
				Packet->onGround = true;
				Packet->mode = 0;

				clientInstance->loopbackPacketSender->sendToServer(Packet);
				delete Packet;
			}/
			
		}
*/
		
		for (uintptr_t i = 0; i < 0xFF; i++) {
			bool* newKey = keyMapAddr + (4 * i); 
			bool newKeyPressed = (*newKey) && (GameData::canUseMoveKeys() || clientInstance->localPlayer == nullptr); // Disable Keybinds when in chat or inventory
			bool* oldKey = keyMap + (4 * i);
			if (newKeyPressed != *oldKey) {
				moduleMgr->onKeyUpdate((int) i, newKeyPressed);
			}
			if(*newKey != *oldKey) // Skip Chat or inventory checks
				TabGui::onKeyUpdate((int)i, *newKey);
		}

		memcpy_s(keyMap, 0xFF * 4, keyMapAddr, 0xFF * 4);
		
		Sleep(10); 
	}
	logF("Uninjecting...");

	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), 1); // Uninject
}

DWORD WINAPI startCheat(LPVOID lpParam)
{
	logF("Starting up...");
	init();

	DWORD procId = GetCurrentProcessId();
	if (!mem.Open(procId, SlimUtils::ProcessAccess::Full))
	{
		logF("Failed to open process, error-code: %i", GetLastError());
		return 1;
	}
	gameModule = mem.GetModule(L"Minecraft.Windows.exe"); // Get Module for Base Address

	MH_Initialize();
	GameData::initGameData(gameModule, &mem);

	TabGui::init();
	Hooks::Init();
	cmdMgr->initCommands();
	moduleMgr->initModules();
	configMgr->init();

	logF("Starting threads...");
	
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) keyThread, lpParam, NULL, NULL); // Checking Keypresses

	ExitThread(0);
}

BOOL __stdcall
DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: //When the injector is called.
	{
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)startCheat, hModule, NULL, NULL);
		DisableThreadLibraryCalls(hModule);
	}
	break;
	case DLL_PROCESS_DETACH:
		isRunning = false;
		configMgr->saveConfig();
		moduleMgr->disable();
		cmdMgr->disable();
		Hooks::Restore();
		logF("Removing logger");
		Logger::Disable();
		
		MH_Uninitialize();

		if (g_Data.getClientInstance()->getLocalPlayer() != nullptr) {
			C_GuiData* guiData = g_Data.getClientInstance()->getGuiData();
			if (guiData != nullptr)
				guiData->displayClientMessageF("%sUninjected!", RED);
		}

		break;
	}
	return TRUE;
}