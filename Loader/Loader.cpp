#include "Loader.h"
#include <vector>

_Offsets Offsets = _Offsets();

uintptr_t EntityList_Base = 0x26C9830;
uintptr_t InGameBase = 0x25E53F8;

uintptr_t* EntityPlayer_Base;
uintptr_t EntityPlayer_Base2;
uintptr_t* GmodeBase;
uintptr_t InGame;

//Entity offsets
uintptr_t Swing = 0x140C;

typedef void(__fastcall * _Attack)(uintptr_t* pThis, uintptr_t* Player);
_Attack Attack;

int bKillAura;
SlimUtils::SlimMem mem;
const SlimUtils::SlimModule* gameModule;
static bool isRunning = true;
C_Entity* localPlayer = 0x0;

// 
// 



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
	if (localPlayer == 0x0)
		return;
	//Declare our target list to define our victims through a dynamic array

	CEntityList* entList = localPlayer->ptrToPtrToEntList->ptrToEntList->entityList;
	//CEntityList* entList = mem.ReadPtr<CEntityList*>(gameModule->ptrBase + 0x26dc038, {0x10, 0xF0, 0x0,0x80,0x30,0x0,0x0});
	uintptr_t listSize = entList->getListSize(); 
	uintptr_t test = (uintptr_t)(entList->firstEntity);
	logF("entList: %llX", entList);
	logF("ptr_entList: %llX", test);

	std::vector<C_Entity*> TargetList;
	//Loop through all our players and retrieve their information
	float maxDist = 10;
	C_Entity* targetEnt = 0x0;
	for (int i = 0; i < listSize; i++)
	{
		//PlayerList[i].ReadInformation(i);
		C_Entity* currentEntity = reinterpret_cast<C_Entity*>(entList->firstEntity + i * sizeof(uintptr_t));
		currentEntity = mem.ReadPtr<C_Entity*>(reinterpret_cast<uintptr_t>(currentEntity), { 0x0 });
		

		if (currentEntity == localPlayer) // Skip Local player
			continue;

		if (currentEntity == 0) 
			continue;
		//targetEnt = currentEntity;
		logF("currentEntity: %llX", currentEntity);
		float dist = currentEntity->eyePos1.dist(localPlayer->eyePos1);
		logF("dist: %f", dist);
		if (dist < maxDist) {
			maxDist = dist;
			targetEnt = currentEntity;
		}
		logF("target: %llX", targetEnt);
		
	}
	//ONLY AIM if we have any enemies
	if (targetEnt != 0x0)
	{
		logF("Pounching %X, dist %f", targetEnt, maxDist);

		
		//AIM at the closest ent, by default aim at ALL times, if you right click hold it switches it off

		{
			if (bKillAura)
			{
				Sleep(20);
				Attack((uintptr_t*)GmodeBase, (uintptr_t*)targetEnt);
				*(int*)((uintptr_t)localPlayer + 0x140C) = 1;
			}
			
		}



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
		if (isKeyPressed('P')) {
			bKillAura = !bKillAura; //true;
			logF("Activating KillAura");
		}
		if (bKillAura)
		{
			localPlayer = mem.ReadPtr<C_Entity*>(gameModule->ptrBase + 0x26dc038, {0x0, 0x10, 0xF0, 0x0});
			localPlayer = mem.ReadPtr<C_Entity*>(reinterpret_cast<uintptr_t>(localPlayer), { 0xE0 });
			logF("local pllayer: %llX", localPlayer);
			GmodeBase = mem.ReadPtr<uintptr_t*>((uintptr_t)(localPlayer->CGameMode), { 0x238,0x18,0x8B8 });

			logF("gmode: %llX", GmodeBase);
			
			
			KillAura();
		}


		Sleep(1);
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