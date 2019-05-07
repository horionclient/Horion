#pragma once

#include <vector>
#include "../../Memory/GameData.h"
#include "Modules/Module.h"
#include "Modules/Killaura.h"
#include "Modules/ESP.h"
#include "Modules/Aimbot.h"
#include "Modules/TriggerBot.h"

class ModuleManager {
private:
	GameData* gameData;
	std::vector<IModule*> moduleList;
public:
	ModuleManager(GameData* gameData);
	void initModules();
	void onTick(C_GameMode* gameMode);
	void onKeyUpdate(int key, bool isDown);
	void onPreRender();
	void onPostRender();
	void onAimbot();
	void onTrigger();
	std::vector<IModule*> getModuleList();
};

extern ModuleManager* moduleMgr;