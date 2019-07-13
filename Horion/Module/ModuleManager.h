#pragma once

#include <vector>
#include <typeinfo>
#include "../../Memory/GameData.h"
#include "Modules/Module.h"
#include "Modules/Killaura.h"
#include "Modules/ESP.h"
#include "Modules/Aimbot.h"
#include "Modules/TriggerBot.h"
#include "Modules/Jetpack.h"
#include "Modules/ChestESP.h"
#include "Modules/RainbowSky.h"
#include "Modules/AirJump.h"
#include "Modules/NoKnockBack.h"
#include "Modules/Step.h"
#include "Modules/Glide.h"
#include "Modules/EditionFaker.h"
#include "Modules/Phase.h"
#include "Modules/Freecam.h"
#include "Modules/AutoSprint.h"
#include "Modules/Tracer.h"
#include "Modules/BowAimbot.h"
#include "Modules/Scaffold.h"
#include "Modules/Speed.h"
#include "Modules/NoFall.h"
#include "Modules/Blink.h"
#include "Modules/Nuker.h"
#include "Modules/InstaBreak.h"
#include "Modules/NoPacket.h"
#include "Modules/Xray.h"
#include "Modules/BedFucker.h"
#include "Modules/NoSlowDown.h"
#include "Modules/InfiniteReach.h"
#include "Modules/AutoTotem.h"
#include "Modules/ClickGuiMod.h"
#include "Modules/ChestStealer.h"
#include "Modules/TabGuiMod.h"
#include "Modules/Fly.h"
#include "Modules/Hitbox.h"
#include "Modules/Reach.h"
#include "Modules/FullBright.h"
#include "../../Utils/Json.hpp"

using json = nlohmann::json;

class ModuleManager {
private:
	GameData* gameData;
	std::vector<IModule*> moduleList;
	bool initialized = false;
public:
	ModuleManager(GameData* gameData);
	void initModules();
	void disable();
	void onLoadConfig(json* conf);
	void onSaveConfig(json* conf);
	void onTick(C_GameMode* gameMode);
	void onKeyUpdate(int key, bool isDown);
	void onPreRender();
	void onPostRender();

	bool isInitialized() { return initialized; };
	std::vector<IModule*>* getModuleList();

	/*
	 *	Use as follows: 
	 *		IModule* mod = moduleMgr.getModule<NoKnockBack>(); 
	 *	Check for nullptr directly after that call, as Hooks::init is called before ModuleManager::initModules !	
	 */
	template<typename TRet>
	IModule* getModule() { 
		if (!isInitialized())
			return nullptr;
		for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
			IModule* mod = *it;
			if (typeid(*mod) == typeid(TRet) || typeid(mod) == typeid(TRet))
				return mod;
		}
		return nullptr;
	};

	// Dont Use this functions unless you absolutely need to. The function above this one works in 99% of cases
	IModule* getModuleByName(const std::string name) {
		if (!isInitialized())
			return nullptr;
		std::string nameCopy = name;
		std::transform(nameCopy.begin(), nameCopy.end(), nameCopy.begin(), ::tolower);

		for (std::vector<IModule*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
			IModule* mod = *it;
			std::string modNameCopy = mod->getModuleName();
			std::transform(modNameCopy.begin(), modNameCopy.end(), modNameCopy.begin(), ::tolower);
			if (modNameCopy == nameCopy)
				return mod;
			
		}
		return nullptr;
	}
};

extern ModuleManager* moduleMgr;
