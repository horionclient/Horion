#include "Loader.h"


SlimUtils::SlimMem mem;
const SlimUtils::SlimModule* gameModule;

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
		DWORD procId = GetCurrentProcessId();
		if (!mem.Open(procId, SlimUtils::ProcessAccess::Full))
		{
			logF("[!!!] Failed to open process, error-code: %i", GetLastError());
			logF("[!!!] You can try starting HBob in Administrator Mode");
			Sleep(5000);
			return 1;
		}
		
	}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}