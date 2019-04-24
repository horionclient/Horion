#include "Loader.h"

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
		
		
	}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}