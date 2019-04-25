#include "Loader.h"
#include <vector>

_Offsets Offsets = _Offsets();

uintptr_t EntityList_Base = 0x26C9830;
uintptr_t InGameBase = 0x25E53F8;

uintptr_t* EntityPlayer_Base;
uintptr_t EntityPlayer_Base2;
C_GameMode* GmodeBase;
uintptr_t InGame;

//Entity offsets
uintptr_t Swing = 0x140C;

typedef void(__fastcall * _Attack)(uintptr_t* pThis, C_Entity* Player);
_Attack Attack;

int bKillAura;
SlimUtils::SlimMem mem;
const SlimUtils::SlimModule* gameModule;
static bool isRunning = true;
C_LocalPlayer* localPlayer = 0x0;

//Compare the distance when sorting the array of Target Enemies, it's called a "sort predicate"
/*struct CompareTargetEnArray
{
	bool operator() (TargetList_t &lhs, TargetList_t &rhs)
	{
		return lhs.m_Distance < rhs.m_Distance;
	}
};*/


void KillAura()
{
	if (localPlayer == 0x0 || !bKillAura)
		return;
	//Declare our target list to define our victims through a dynamic array
	C_EntityList* entList = localPlayer->getEntityList();
	size_t listSize = entList->getListSize();

	//Loop through all our players and retrieve their information
	float maxDist = 10;
	static std::vector <C_Entity*> targetList;
	targetList.clear();
	for (size_t i = 0; i < listSize; i++)
	{
		C_Entity* currentEntity = entList->get(i);

		if (currentEntity == 0)
			break;

		if (currentEntity == localPlayer) // Skip Local player
			continue;

		float dist = currentEntity->eyePos1.dist(localPlayer->eyePos1);

		if (dist < maxDist) {
			targetList.push_back(currentEntity);
		}
	}

	// Attack all entitys in targetList 
	for (int i = 0; i < targetList.size(); i++)
	{
		Attack((uintptr_t*)GmodeBase, targetList[i]);
		localPlayer->swing = 1;
		logF("swing: %llX", &localPlayer->ticksAlive);
		logF("wid: %llX", &localPlayer->width);
		logF("tt: %llX", &localPlayer->name);
		logF("tt i: %llX", &localPlayer->name.inlineText);
		logF("tt p: %llX", &localPlayer->name.pText);
		logF("tt s: %llX", &localPlayer->name.textLength);
		logF("bodyYaw: %llX", &localPlayer->bodyYaw);
		
	}

	
}

bool isKeyDown(int key) {
	static constexpr uintptr_t keyMapOffset = 0x26866E0; // Found via scan, static value
	// All keys are mapped there as bools, though 4 bytes in size
	// key0 00 00 00 key1 00 00 00 key2 00 00 00 ...
	return *reinterpret_cast<bool*>(gameModule->ptrBase + keyMapOffset + (key * 0x4));
}

bool isKeyPressed(int key) {
	if (isKeyDown(key)) {
		while (isKeyDown(key))
			Sleep(2);
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
		if (isKeyPressed('P')) {
			bKillAura = !bKillAura; //true;
			logF("%s KillAura", bKillAura ? "Activating" : "Deactivating");
		}
		if (bKillAura)
		{
			localPlayer = mem.ReadPtr<C_LocalPlayer*>(gameModule->ptrBase + 0x26dc038, { 0x0, 0x10, 0xF0, 0x0, 0xE0 });
			if (localPlayer != 0x0) {
				GmodeBase = localPlayer->getCGameMode();
				KillAura();
			}
		
			
		}


		Sleep(50); // 1/20
	}

	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), 1); // Uninject
	return 1;
}

DWORD WINAPI startCheat(LPVOID lpParam)
{
	logF("Starting cheat...");
	DWORD procId = GetCurrentProcessId();
	if (!mem.Open(procId, SlimUtils::ProcessAccess::Full))
	{
		logF("Failed to open process, error-code: %i", GetLastError());
		return 1;
	}
	gameModule = mem.GetModule(L"Minecraft.Windows.exe"); // Get Module for Base Address
	uintptr_t* GameModeAttack = (uintptr_t*)(gameModule->ptrBase + 0x222cbe0);
	Attack = (_Attack)(*GameModeAttack);
	bKillAura = false;
	logF("Starting threads...");

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)keyThread, lpParam, NULL, NULL); // Checking Keypresses
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