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

typedef void(__fastcall * _Attack)(void* pThis, void* Player);
_Attack Attack;

int bKillAura;
SlimUtils::SlimMem mem;
const SlimUtils::SlimModule* gameModule;
static bool isRunning = true;

struct MyPlayer_t
{
	C_Entity* CLocalPlayer; //address of our ent
	vec3_t Eye_Position;
	//float m_hitbox;
	//vec3 Position;
	void ReadInformation()
	{
		EntityPlayer_Base = (uintptr_t*)(mem.ReadPtr<uintptr_t>(gameModule->ptrBase + EntityList_Base, Offsets.EntityListOffsets));
		
		CLocalPlayer = reinterpret_cast<C_Entity*>(EntityPlayer_Base);

		GmodeBase = (uintptr_t*)(mem.ReadPtr<uintptr_t>((uintptr_t)(CLocalPlayer->CGameMode), Offsets.GmodeOffsets));
		Attack = (_Attack)(gameModule->ptrBase + 0x222CBE0);
		
		Eye_Position = CLocalPlayer->eyePos1;
	}
}MyPlayer;



struct PlayerList_t
{
	C_Entity* CBaseEntity;
	uintptr_t* pBaseEntity;
	vec3_t Eye_Position;

	void ReadInformation(int Player)
	{
		//Get Address of Entity
		pBaseEntity = EntityPlayer_Base + (Player * sizeof(uintptr_t));
		CBaseEntity = reinterpret_cast<C_Entity*>(EntityPlayer_Base + (Player * sizeof(uintptr_t)));
		Eye_Position = CBaseEntity->eyePos1;
	}
};

struct TargetList_t
{
	C_Entity *CBaseEntity;
	float m_Distance;

	TargetList_t() {}

	TargetList_t(C_Entity* BaseEntity)
	{
		CBaseEntity = BaseEntity;
	}

};
int SetEntityListSize()
{
	throw std::runtime_error("Not implemented yet");
	//EntityPlayer_Base2 = FindDMAAddy((gameModule->ptrBase + EntityList_Base), { EntityListOffsets[0],EntityListOffsets[1]
	//,EntityListOffsets[2] });
	uintptr_t* FirstEntity_p = EntityPlayer_Base;
	uintptr_t* LastEntity_p;
	
	LastEntity_p = (uintptr_t*)(EntityPlayer_Base2 + 0x8);
	int i = (int)((LastEntity_p - FirstEntity_p) / sizeof(uintptr_t));
	return i;
}
int CheckEntity(uintptr_t* player, int i)
{
	uintptr_t* FirstEntity_p = EntityPlayer_Base;
	uintptr_t* pUnknowEntity = player;
	return (((int)((pUnknowEntity - FirstEntity_p) / sizeof(uintptr_t))) == i);
}


//Compare the distance when sorting the array of Target Enemies, it's called a "sort predicate"
struct CompareTargetEnArray
{
	bool operator() (TargetList_t &lhs, TargetList_t &rhs)
	{
		return lhs.m_Distance < rhs.m_Distance;
	}
};


void KillAura()
{
	//Declare our target list to define our victims through a dynamic array
	int size = 100;
	//std::vector<PlayerList_t> PlayerList;
	PlayerList_t* PlayerList = new PlayerList_t[size];
	std::vector<TargetList_t> TargetList;
	//Loop through all our players and retrieve their information
	int targetLoop = 0;
	for (int i = 0; i < size; i++)
	{
		PlayerList[i].ReadInformation(i);

		if (PlayerList[i].CBaseEntity == MyPlayer.CLocalPlayer) // Skip Local player
			continue;

		if (!CheckEntity((PlayerList[i].pBaseEntity), i)) // Ghetto code
			continue;

		if (MyPlayer.Eye_Position.dist(PlayerList[i].Eye_Position) > 10) // Throw away if distance > 10
			continue;
	

		TargetList.push_back(TargetList_t(PlayerList[i].CBaseEntity));
		//Increment to advance the array for the next iteration
		targetLoop++;
	}
	//ONLY AIM if we have any enemies
	if (targetLoop > 0)
	{
		{
			//bKillAura = 0;
		}
		//SORT ENEMIES ARRAY BY DISTANCE by using our sort predicate

		//std::sort(TargetList.begin(), (TargetList.begin() + targetLoop), CompareTargetEnArray());

		//ReadProcessMemory(hProcess, (BYTE*)(InGame), &In_Game, sizeof(int), nullptr);
		//AIM at the closest ent, by default aim at ALL times, if you right click hold it switches it off
		/*for (int i = 0; i < TargetList.size() && (*(TargetList[i].CBaseEntity)) != 0 && bKillAura
			; i++)//  && In_Game == 1; i++)
		{
			if (bKillAura)
			{
				Sleep(120);
				/*Attack((void*)GmodeBase, (void*)(*(TargetList[i].CBaseEntity)));
				*(int*)((*(MyPlayer.CLocalPlayer)) + 0x140C) = 1;*/
				//CallAttackFunc(hProcess, GmodeBase, TargetList[i].CBaseEntity, MyPlayer.CLocalPlayer);
			//}
			//CallAttackFunc(hProcess, GmodeBase, TargetList[i].CBaseEntity, MyPlayer.CLocalPlayer);
		//}



	}
	targetLoop = 0;
	delete[] PlayerList;
	
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
			bKillAura = !bKillAura;
			logF("Activating KillAura");
		}
		if (bKillAura)
		{
			MyPlayer.ReadInformation();
			//KillAura();
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