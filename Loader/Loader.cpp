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
		localPlayer->swingArm();
	}

	
}

bool isKeyDown(int key) {
	static uintptr_t keyMapOffset = 0x0;
	if (keyMapOffset == 0x0) {
		uintptr_t sigOffset = Utils::FindSignature("48 8D 0D ?? ?? ?? ?? 89 3C 81 E9");
		if (sigOffset != 0x0) {
			uint32_t offset = *reinterpret_cast<uint32_t*>((sigOffset + 3)); // Get Offset from code
			keyMapOffset = sigOffset - gameModule->ptrBase + offset + /*length of instruction*/ 7; // Offset is relative
#ifdef _DEBUG
			logF("Recovered KeyMapOffset: %X", keyMapOffset);
#endif
		}
		else
			logF("!!!KeyMap not located!!!");
	}
	// All keys are mapped as bools, though aligned as ints (4 byte)
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

	logF("Starting threads...");

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)keyThread, lpParam, NULL, NULL); // Checking Keypresses
	logF("Started!");

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
		logF("Starting cheat");

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)startCheat, hModule, NULL, NULL);
		DisableThreadLibraryCalls(hModule);
	}
	break;
	case DLL_PROCESS_DETACH:
		logF("Removing logger");
		DisableLogger();
		break;
	}
	return TRUE;
}