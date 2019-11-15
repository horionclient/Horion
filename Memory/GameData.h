#pragma once

#include <set>
#include "../SDK/CClientInstance.h"
#include "../SDK/CGameMode.h"
#include "../SDK/CChestBlockActor.h"
#include "../SDK/CHIDController.h"
#include "../SDK/CMoveInputHandler.h"
#include "../SDK/CRakNetInstance.h"
#include "../Utils/TextFormat.h"
#include "SlimMem.h"
#include <map>


class GameData {
private:
	C_ClientInstance* clientInstance = 0;
	C_LocalPlayer* localPlayer = 0;
	C_GameMode* gameMode = 0;
	C_EntityList* entityList = 0;
	C_HIDController* hidController = 0;
	C_RakNetInstance* raknetInstance = 0;
	HMODULE hDllInst = 0;
	std::set<std::shared_ptr<AABB>> chestList = std::set<std::shared_ptr<AABB>>();
	
	const SlimUtils::SlimModule* gameModule = 0;
	SlimUtils::SlimMem* slimMem;
	bool shouldTerminateB = false;
	bool shouldHideB = false;
	LARGE_INTEGER lastUpdate;
	static void retrieveClientInstance();
public:
	static bool canUseMoveKeys();
	static bool isKeyDown(int key);
	static bool isKeyPressed(int key);
	static bool isRightClickDown();
	static bool isLeftClickDown();
	static bool isWheelDown();
	static bool shouldTerminate();
	static bool shouldHide();
	static void hide();
	static void terminate();
	static void updateGameData(C_GameMode* gameMode);
	static void initGameData(const SlimUtils::SlimModule* gameModule, SlimUtils::SlimMem* slimMem, HMODULE hDllInst);
	static void addChestToList(C_ChestBlockActor * ChestBlock2);
	static void EntityList_tick(C_EntityList * list);
	static void setHIDController(C_HIDController* Hid);
	static void setRakNetInstance(C_RakNetInstance* raknet);

	inline HMODULE getDllModule() { return hDllInst; };
	inline C_ClientInstance* getClientInstance() { return clientInstance; };
	inline C_GuiData* getGuiData() { return clientInstance->getGuiData(); };
	inline C_LocalPlayer* getLocalPlayer() {
		
		localPlayer = clientInstance->getLocalPlayer();
		if (localPlayer == nullptr)
			gameMode = nullptr;
		return localPlayer;
	};
	C_LocalPlayer** getPtrLocalPlayer() {
		return &localPlayer;
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
	C_HIDController** getHIDController() { return &hidController; };
	C_RakNetInstance* getRakNetInstance() { return raknetInstance; };
	std::set<std::shared_ptr<AABB>>* getChestList() { return &chestList; };

	inline LARGE_INTEGER getLastUpdateTime() { return lastUpdate; };

	void forEachEntity(void(*callback) (C_Entity*,bool));
};


extern GameData g_Data;