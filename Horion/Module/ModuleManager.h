#pragma once

#include <vector>
#include "../../Memory/GameData.h"
#include "Modules/Module.h"
#include "Modules/Killaura.h"

class ModuleManager {
private:
	GameData* gameData;
	std::vector<IModule*> moduleList;
public:
	ModuleManager(GameData* gameData);
	void initModules();
	void onTick(C_GameMode* gameMode);
};

extern ModuleManager* moduleMgr;