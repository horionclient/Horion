#pragma once

#include <set>
#include "../SDK/CClientInstance.h"
#include "../SDK/CGameMode.h"
#include "../SDK/CChestBlockActor.h"
#include "../SDK/CHIDController.h"
#include "../SDK/CMoveInputHandler.h"
#include "../Utils/TextFormat.h"
#include "SlimMem.h"
#include <map>


class GameData {
private:
	C_ClientInstance* clientInstance = 0;
	C_LocalPlayer* localPlayer;
	C_GameMode* gameMode;
	C_EntityList* entityList = 0;
	C_HIDController* hidController;
	C_MoveInputHandler* inputHandler;
	C_ChestScreenController* chestScreen;
	std::set<std::shared_ptr<AABB>> chestList = std::set<std::shared_ptr<AABB>>();
	
	const SlimUtils::SlimModule* gameModule;
	SlimUtils::SlimMem* slimMem;
	bool shouldTerminateB = false;
	static void retrieveClientInstance();
public:
	static bool canUseMoveKeys();
	static bool isKeyDown(int key);
	static bool isKeyPressed(int key);
	static bool isLeftClickDown();
	static bool shouldTerminate();
	static void terminate();
	static void updateGameData(C_GameMode* gameMode);
	static void initGameData(const SlimUtils::SlimModule* gameModule, SlimUtils::SlimMem* slimMem);
	static void addChestToList(C_ChestBlockActor * ChestBlock2);
	static void EntityList_tick(C_EntityList * list);
	static void setHIDController(C_HIDController* Hid);
	static void setMoveInputHandler(C_MoveInputHandler* handler);
	static void setChestScreenController(C_ChestScreenController* chestScreenController);

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
	C_MoveInputHandler* getInputHandler() { return inputHandler; };
	C_ChestScreenController** getChestScreenController() { return &chestScreen; };
	std::set<std::shared_ptr<AABB>>* getChestList() { return &chestList; };

	void forEachEntity(void(*callback) (C_Entity*));
};


extern GameData g_Data;