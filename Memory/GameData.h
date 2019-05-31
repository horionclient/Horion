#pragma once

#include <set>
#include "../SDK/CClientInstance.h"
#include "../SDK/CGameMode.h"
#include "../SDK/CChestBlockActor.h"
#include "../Utils/TextFormat.h"
#include "SlimMem.h"

class GameData {
private:
	C_ClientInstance* clientInstance;
	C_LocalPlayer* localPlayer;
	C_GameMode* gameMode;
	C_EntityList* entityList;
	uintptr_t hid;
	std::set<std::shared_ptr<AABB>> chestList = std::set<std::shared_ptr<AABB>>();
	const SlimUtils::SlimModule* gameModule;
	SlimUtils::SlimMem* slimMem;
	bool shouldTerminateB = false;
	bool shouldHide = false;
	bool onTheRight = true;
	bool LockKeyBind = false;
	static void retrieveClientInstance();
public:
	static bool canUseMoveKeys();
	static bool shouldLock();
	static void Lock();
	static bool shouldOnTheRight();
	static void OnTheRight();
	static bool ShouldHide();
	static void Hide();
	static bool isKeyDown(int key);
	static bool isKeyPressed(int key);
	static bool isRightButtonPressed();
	static bool shouldTerminate();
	static void terminate();
	static void updateGameData(C_GameMode* gameMode);
	static void initGameData(const SlimUtils::SlimModule* gameModule, SlimUtils::SlimMem* slimMem);
	static void addChestToList(C_ChestBlockActor * ChestBlock2);
	static void EntityList_tick(C_EntityList * list);
	static void addHIDController(void* Hid);

	

	C_ClientInstance* getClientInstance() { return clientInstance; };
	C_GuiData* getGuiData() { return clientInstance->getGuiData(); };
	C_LocalPlayer* getLocalPlayer() {
		
		localPlayer = clientInstance->getLocalPlayer();
		if (localPlayer == nullptr)
			gameMode = nullptr;
		return localPlayer;
	};
	bool isInGame() {
		return localPlayer != nullptr;
	}
	const SlimUtils::SlimModule* getModule() {
		return gameModule;
	};
	const SlimUtils::SlimMem* getSlimMem() {
		return slimMem;
	};
	C_GameMode* getCGameMode() { return gameMode; };
	C_EntityList* getEntityList() { return entityList; };
	uintptr_t getHIDController() { return hid; };
	std::set<std::shared_ptr<AABB>>* getChestList() { return &chestList; };
};


extern GameData g_Data;