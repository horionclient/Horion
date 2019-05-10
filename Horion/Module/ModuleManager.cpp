#include "ModuleManager.h"

ModuleManager::ModuleManager(GameData * gameData)
{
	this->gameData = gameData;
}

void ModuleManager::initModules()
{
	this->moduleList.push_back(new Killaura());
	this->moduleList.push_back(new ESP());
	this->moduleList.push_back(new Jetpack());
	this->moduleList.push_back(new Aimbot());
	this->moduleList.push_back(new TriggerBot());
	this->moduleList.push_back(new ChestESP());
	this->moduleList.push_back(new NoKnockBack());
	this->moduleList.push_back(new RainbowSky());

	this->getModule<RainbowSky>()->setEnabled(true);
}

void ModuleManager::onTick(C_GameMode * gameMode)
{
	for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		IModule* mod = *it;
		if (mod->isEnabled())
			mod->onTick(gameMode);
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

std::vector<IModule*>* ModuleManager::getModuleList()
{
	return &moduleList;
}

ModuleManager* moduleMgr = new ModuleManager(&g_Data);