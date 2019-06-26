#include "Loader.h"

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

	bool* keyMap = static_cast<bool*>(malloc(0xFF * 4 + 0x4));
	if(keyMap == 0)
		throw std::exception("Keymap not allocated");

	uintptr_t clickMap = reinterpret_cast<uintptr_t>(malloc(5));
	if (clickMap == 0)
		throw std::exception("Clickmap not allocated");

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

	C_HIDController** hidController = g_Data.getHIDController();

	while (isRunning) {
		if (GameData::isKeyDown('L') && GameData::isKeyDown(VK_CONTROL) || GameData::shouldTerminate()) { // Press L to uninject
			isRunning = false;
			break;
		}
		
		for (uintptr_t i = 0; i < 0xFF; i++) {
			bool* newKey = keyMapAddr + (4 * i); 
			bool newKeyPressed = (*newKey) && GameData::canUseMoveKeys(); // Disable Keybinds when in chat or inventory
			bool* oldKey = keyMap + (4 * i);
			if (newKeyPressed != *oldKey) {
				moduleMgr->onKeyUpdate((int) i, newKeyPressed);
			}
			if(*newKey != *oldKey) // Skip Chat or inventory checks
				TabGui::onKeyUpdate((int)i, *newKey);
		}

		if (*hidController != 0) {
			
			for (uintptr_t key = 0; key < 5; key++) {
				bool newKey = (*hidController)->clickMap[key];
				bool* oldKey = reinterpret_cast<bool*>(clickMap + key);
				if (newKey != *oldKey) {
					ClickGui::onMouseClickUpdate((int)key, newKey);
				}
			}

			memcpy(reinterpret_cast<void*>(clickMap), &(*hidController)->leftClickDown, 5);
		}
		

		memcpy_s(keyMap, 0xFF * 4, keyMapAddr, 0xFF * 4);
		
		Sleep(2); 
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
	Target::init(g_Data.getPtrLocalPlayer());

	TabGui::init();
	ClickGui::init();
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
	case DLL_PROCESS_ATTACH:
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