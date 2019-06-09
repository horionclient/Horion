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
	this->moduleList.push_back(new AirJump());
	this->moduleList.push_back(new Step());
	this->moduleList.push_back(new Glide());
	this->moduleList.push_back(new EditionFaker());
	this->moduleList.push_back(new Phase());
	this->moduleList.push_back(new Freecam());
	this->moduleList.push_back(new AutoSprint());
	this->moduleList.push_back(new Tracer());
	this->moduleList.push_back(new BowAimbot());
	this->moduleList.push_back(new Scaffold());
	this->moduleList.push_back(new NoFall());
	this->moduleList.push_back(new Blink());
	this->moduleList.push_back(new Nuker());
	this->moduleList.push_back(new InstaBreak());
	this->moduleList.push_back(new NoPacket());
	this->moduleList.push_back(new Speed());
	this->moduleList.push_back(new Xray());
	this->moduleList.push_back(new BedFucker());
	this->moduleList.push_back(new MobESP());
	this->moduleList.push_back(new MobAura());
	initialized = true;

	this->getModule<RainbowSky>()->setEnabled(true);
}

void ModuleManager::disable()
{
	for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		IModule* mod = *it;
		if (mod->isEnabled())
			mod->setEnabled(false);
	}
}

void ModuleManager::onLoadConfig(json * conf)
{
	if (!isInitialized())
		return;
	for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		IModule* mod = *it;
		mod->onLoadConfig(conf);
	}
}

void ModuleManager::onSaveConfig(json * conf)
{
	if (!isInitialized())
		return;
	for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		IModule* mod = *it;
		mod->onSaveConfig(conf);
	}
}

void ModuleManager::onTick(C_GameMode * gameMode)
{
	if (!isInitialized())
		return;
	for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		IModule* mod = *it;
		if (mod->isEnabled())
			mod->onTick(gameMode);
	}
}

void ModuleManager::onKeyUpdate(int key, bool isDown)
{
	if (!isInitialized())
		return;
	for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		IModule* mod = *it;
		mod->onKeyUpdate(key, isDown);
	}
}

void ModuleManager::onPreRender()
{
	if (!isInitialized())
		return;
	for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		IModule* mod = *it;
		if (mod->isEnabled())
			mod->onPreRender();
	}
}

void ModuleManager::onPostRender()
{
	if (!isInitialized())
		return;
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