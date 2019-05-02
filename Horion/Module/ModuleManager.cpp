#include "ModuleManager.h"

ModuleManager::ModuleManager(GameData * gameData)
{
	this->gameData = gameData;
}

void ModuleManager::initModules()
{
	this->moduleList.push_back(new Killaura());
}

void ModuleManager::onTick(C_GameMode * gameMode)
{
	for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		IModule* mod = *it;
		if (mod->isEnabled())
			mod->onTick(gameMode);
	}
}

ModuleManager* moduleMgr = new ModuleManager(&g_Data);