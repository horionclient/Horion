#include "ModuleManager.h"

ModuleManager::ModuleManager(GameData * gameData)
{
	this->gameData = gameData;
}

void ModuleManager::initModules()
{
	this->moduleList.push_back(new Killaura());
	this->moduleList.push_back(new ESP());
	this->moduleList.push_back(new Aimbot());
	this->moduleList.push_back(new TriggerBot());
}

void ModuleManager::onTick(C_GameMode * gameMode)
{
	for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		IModule* mod = *it;
		if (mod->isEnabled())
			mod->onTick(gameMode);
	}
}
void ModuleManager::onTrigger()
{
	for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		IModule* mod = *it;
		if (mod->isEnabled())
			mod->onTrigger();
	}
}
void ModuleManager::onAimbot()
{
	for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		IModule* mod = *it;
		if (mod->isEnabled())
			mod->onAimbot();
	}
}

void ModuleManager::onKeyUpdate(int key, bool isDown)
{
	for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		IModule* mod = *it;
		mod->onKeyUpdate(key, isDown);
	}
}

void ModuleManager::onPreRender()
{
	for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		IModule* mod = *it;
		if (mod->isEnabled())
			mod->onPreRender();
	}
}

void ModuleManager::onPostRender()
{
	for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		IModule* mod = *it;
		if (mod->isEnabled())
			mod->onPostRender();
	}
}

std::vector<IModule*> ModuleManager::getModuleList()
{
	return moduleList;
}

ModuleManager* moduleMgr = new ModuleManager(&g_Data);