#include "GameData.h"

GameData g_Data;

void GameData::updateGameData(C_GameMode * gameMode)
{
	g_Data.gameMode = gameMode;
	g_Data.localPlayer = g_Data.getLocalPlayer();
}

void GameData::initGameData(const SlimUtils::SlimModule* gameModule, SlimUtils::SlimMem* slimMem)
{
	g_Data.gameModule = gameModule;
	g_Data.slimMem = slimMem;
	g_Data.clientInstance = slimMem->ReadPtr<C_ClientInstance*>(gameModule->ptrBase + 0x26e1108, { 0x0, 0x10, 0xF0, 0x0 });
}
