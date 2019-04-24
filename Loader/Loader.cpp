#include "Loader.h"


SlimUtils::SlimMem mem;
const SlimUtils::SlimModule* gameModule;
static bool isRunning = true;


bool isKeyDown(int key) {
	
	//return GetAsyncKeyState(key) & 0x8000;
	//logF("KEy: %X", gameModule->ptrBase + 0x26866E0 + (key * 0x4));
	return mem.Read<bool>(gameModule->ptrBase + 0x26866E0 + (key * 0x4));
}

bool isKeyPressed(int key) {
	if (isKeyDown(key)) {
		
		while (isKeyDown(key))
			Sleep(5);
		return true;
	}
	return false;
}

DWORD WINAPI keyThread(LPVOID lpParam)
{
	logF("Key thread started");
	while (isRunning) {
		if (isKeyPressed('L')) { // Press L to uninject
			isRunning = false;
			logF("Uninjecting...");
			break;
		}

		Sleep(1);
	}

	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), 1);
	
	return 1;
}

DWORD WINAPI startCheat(LPVOID lpParam)
{
	logF("Starting cheat..."); 
	DWORD procId = GetCurrentProcessId();
	if (!mem.Open(procId, SlimUtils::ProcessAccess::Full))
	{
		logF("[!!!] Failed to open process, error-code: %i", GetLastError());
		logF("[!!!] You can try starting Horion in Administrator Mode");
		return 1;
	}
	gameModule = mem.GetModule(L"Minecraft.Windows.exe");

	logF("Starting threads...");

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)keyThread, lpParam, NULL, NULL);
	logF("Started!");

	ExitThread(0);
	return 1;
}

BOOL __stdcall
DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: //When the injector is called.
	{
		logF("Starting cheat");

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)startCheat, hModule, NULL, NULL);
		DisableThreadLibraryCalls(hModule);
	}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}