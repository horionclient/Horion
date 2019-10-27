#pragma once

#include <vector>
#include <typeinfo>
#include "../../Memory/GameData.h"
#include "Modules/Module.h"
#include "Modules/HudModule.h"
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
#include "Modules/Fly.h"
#include "Modules/Hitbox.h"
#include "Modules/Reach.h"
#include "Modules/FullBright.h"
#include "Modules/AirSwim.h"
#include "Modules/InventoryMove.h"
#include "Modules/HighJump.h"
#include "Modules/NoWeb.h"
#include "Modules/FastLadder.h"
#include "Modules/StackableItem.h"
#include "Modules/AutoSneak.h"
#include "Modules/FastEat.h"
#include "Modules/Jesus.h"
#include "Modules/AutoArmor.h"
#include "Modules/InfiniteBlockReach.h"
#include "Modules/AutoClicker.h"
#include "Modules/NameTags.h"
#include "Modules/Criticals.h"
#include "Modules/Bhop.h"
#include "Modules/Tower.h"
#include "Modules/AutoGapple.h"
#include "Modules/BugUp.h"
#include "Modules/MidClick.h"
#include "Modules/ClickTP.h"
#include "Modules/NoFriends.h"
#include "Modules/Spammer.h"
#include "Modules/ChestAura.h"
#include "Modules/AntiBot.h"
#include "../../Utils/Json.hpp"

using json = nlohmann::json;

class ModuleManager {
private:
	GameData* gameData;
	std::vector<IModule*> moduleList;
	bool initialized = false;
public:
	~ModuleManager();
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

	int getModuleCount();
	int getEnabledModuleCount();

	/*
	 *	Use as follows: 
	 *		IModule* mod = moduleMgr.getModule<NoKnockBack>(); 
	 *	Check for nullptr directly after that call, as Hooks::init is called before ModuleManager::initModules !	
	 */
	template<typename TRet>
	TRet* getModule() {
		if (!isInitialized())
			return nullptr;
		for (auto pMod : moduleList) {
			if (auto pRet = dynamic_cast<std::remove_pointer<TRet>::type*>(pMod))
				return pRet;
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
