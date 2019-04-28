#pragma once

#include "../SDK/CClientInstance.h"
#include "../SDK/CGameMode.h"
#include "SlimMem.h"


class GameData {
private:
	C_ClientInstance* clientInstance;
	C_LocalPlayer* localPlayer;
	C_GameMode* gameMode;
	const SlimUtils::SlimModule* gameModule;
	SlimUtils::SlimMem* slimMem;
public:
	static bool isKeyDown(int key);
	static bool isKeyPressed(int key);
	static void updateGameData(C_GameMode* gameMode);
	static void initGameData(const SlimUtils::SlimModule* gameModule, SlimUtils::SlimMem* slimMem);

	C_ClientInstance* getClientInstance() { return clientInstance; };
	C_LocalPlayer* getLocalPlayer() {
		localPlayer = clientInstance->getLocalPlayer();
		if (localPlayer == nullptr)
			gameMode = nullptr;
		return localPlayer;
	};
	C_GameMode* getCGameMode() { return gameMode; };
};


extern GameData g_Data;