#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <typeinfo>
#include <vector>

#include "../../Memory/GameData.h"
#include "Horion/Module/Modules/Combat/Aimbot.h"
#include "Horion/Module/Modules/Combat/AutoClicker.h"
#include "Horion/Module/Modules/Combat/BowAimbot.h"
#include "Horion/Module/Modules/Combat/Criticals.h"
#include "Horion/Module/Modules/Combat/CrystalAura.h"
#include "Horion/Module/Modules/Combat/Hitbox.h"
#include "Horion/Module/Modules/Combat/InfiniteReach.h"
#include "Horion/Module/Modules/Combat/Killaura.h"
#include "Horion/Module/Modules/Combat/Reach.h"
#include "Horion/Module/Modules/Combat/Teams.h"
#include "Horion/Module/Modules/Combat/TriggerBot.h"
#include "Horion/Module/Modules/Misc/AntiImmobile.h"
#include "Horion/Module/Modules/Misc/Blink.h"
#include "Horion/Module/Modules/Misc/Crasher.h"
#include "Horion/Module/Modules/Misc/Derp.h"
#include "Horion/Module/Modules/Misc/EditionFaker.h"
#include "Horion/Module/Modules/Misc/Freecam.h"
#include "Horion/Module/Modules/Misc/Fucker.h"
#include "Horion/Module/Modules/Misc/Godmode.h"
#include "Horion/Module/Modules/Misc/NoPacket.h"
#include "Horion/Module/Modules/Misc/NoPaintingCrash.h"
#include "Horion/Module/Modules/Misc/Spammer.h"
#include "Horion/Module/Modules/Misc/Teleport.h"
#include "Horion/Module/Modules/Misc/Timer.h"
#include "Horion/Module/Modules/Movement/AirJump.h"
#include "Horion/Module/Modules/Movement/AirStuck.h"
#include "Horion/Module/Modules/Movement/AirSwim.h"
#include "Horion/Module/Modules/Movement/AntiVoid.h"
#include "Horion/Module/Modules/Movement/AutoGapple.h"
#include "Horion/Module/Modules/Movement/AutoSneak.h"
#include "Horion/Module/Modules/Movement/AutoSprint.h"
#include "Horion/Module/Modules/Movement/Bhop.h"
#include "Horion/Module/Modules/Movement/CubeGlide.h"
#include "Horion/Module/Modules/Movement/FastLadder.h"
#include "Horion/Module/Modules/Movement/Fly.h"
#include "Horion/Module/Modules/Movement/FollowPathModule.h"
#include "Horion/Module/Modules/Movement/Glide.h"
#include "Horion/Module/Modules/Movement/HighJump.h"
#include "Horion/Module/Modules/Movement/InventoryMove.h"
#include "Horion/Module/Modules/Movement/Jesus.h"
#include "Horion/Module/Modules/Movement/Jetpack.h"
#include "Horion/Module/Modules/Movement/NoFall.h"
#include "Horion/Module/Modules/Movement/NoSlowDown.h"
#include "Horion/Module/Modules/Movement/NoWeb.h"
#include "Horion/Module/Modules/Movement/Phase.h"
#include "Horion/Module/Modules/Movement/Speed.h"
#include "Horion/Module/Modules/Movement/Step.h"
#include "Horion/Module/Modules/Movement/Velocity.h"
#include "Horion/Module/Modules/Player/AntiBot.h"
#include "Horion/Module/Modules/Player/AutoArmor.h"
#include "Horion/Module/Modules/Player/AutoTotem.h"
#include "Horion/Module/Modules/Player/ChestAura.h"
#include "Horion/Module/Modules/Player/ChestStealer.h"
#include "Horion/Module/Modules/Player/FastEat.h"
#include "Horion/Module/Modules/Player/InfiniteBlockReach.h"
#include "Horion/Module/Modules/Player/InventoryCleaner.h"
#include "Horion/Module/Modules/Player/MidClick.h"
#include "Horion/Module/Modules/Player/Nbt.h"
#include "Horion/Module/Modules/Player/NoFriends.h"
#include "Horion/Module/Modules/Player/NoSwing.h"
#include "Horion/Module/Modules/Player/StackableItem.h"
#include "Horion/Module/Modules/Visual/ChestESP.h"
#include "Horion/Module/Modules/Visual/ClickGuiMod.h"
#include "Horion/Module/Modules/Visual/ESP.h"
#include "Horion/Module/Modules/Visual/Freelook.h"
#include "Horion/Module/Modules/Visual/FullBright.h"
#include "Horion/Module/Modules/Visual/HudModule.h"
#include "Horion/Module/Modules/Visual/NameTags.h"
#include "Horion/Module/Modules/Visual/NightMode.h"
#include "Horion/Module/Modules/Visual/NoHurtcam.h"
#include "Horion/Module/Modules/Visual/RainbowSky.h"
#include "Horion/Module/Modules/Visual/Tracer.h"
#include "Horion/Module/Modules/Visual/Xray.h"
#include "Horion/Module/Modules/Visual/Zoom.h"
#include "Horion/Module/Modules/World/InstaBreak.h"
#include "Horion/Module/Modules/World/Nuker.h"
#include "Horion/Module/Modules/World/Scaffold.h"
#include "Horion/Module/Modules/World/Tower.h"
#include "Modules/Module.h"

#ifdef _DEBUG
#include "Modules/PacketLogger.h"
#include "Modules/TestModule.h"
#endif


class ModuleManager {
private:
	GameData* gameData;
	std::vector<std::shared_ptr<IModule>> moduleList;
	bool initialized = false;
	std::shared_mutex moduleListMutex;

public:
	~ModuleManager();
	ModuleManager(GameData* gameData);
	void initModules();
	void disable();
	void onLoadConfig(void* conf);
	void onSaveConfig(void* conf);
	void onTick(C_GameMode* gameMode);
	void onAttack(C_Entity* attackedEnt);

	void onKeyUpdate(int key, bool isDown);
	void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	void onPostRender(C_MinecraftUIRenderContext* renderCtx);
	void onLevelRender();
	void onMove(C_MoveInputHandler* handler);
	void onSendPacket(C_Packet*);

	std::shared_lock<std::shared_mutex> lockModuleList() { return std::shared_lock(this->moduleListMutex); }
	std::unique_lock<std::shared_mutex> lockModuleListExclusive() { return std::unique_lock(this->moduleListMutex); }
	
	std::shared_mutex* getModuleListLock() { return &this->moduleListMutex; }

	bool isInitialized() { return initialized; };
	std::vector<std::shared_ptr<IModule>>* getModuleList();

	int getModuleCount();
	int getEnabledModuleCount();

	/*
	 *	Use as follows: 
	 *		IModule* mod = moduleMgr.getModule<NoKnockBack>(); 
	 *	Check for nullptr directly after that call, as Hooks::init is called before ModuleManager::initModules !	
	 */
	template <typename TRet>
	TRet* getModule() {
		if (!isInitialized())
			return nullptr;
		auto lock = this->lockModuleList();
		for (auto pMod : moduleList) {
			if (auto pRet = dynamic_cast<typename std::remove_pointer<TRet>::type*>(pMod.get())){
				
				return pRet;
			}
		}
		return nullptr;
	};

	// Dont Use this functions unless you absolutely need to. The function above this one works in 99% of cases
	std::optional<std::shared_ptr<IModule>> getModuleByName(const std::string name) {
		if (!isInitialized())
			return nullptr;
		std::string nameCopy = name;
		std::transform(nameCopy.begin(), nameCopy.end(), nameCopy.begin(), ::tolower);
		
		auto lock = this->lockModuleList();
		for (std::vector<std::shared_ptr<IModule>>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
			std::shared_ptr<IModule> mod = *it;
			std::string modNameCopy = mod->getRawModuleName();
			std::transform(modNameCopy.begin(), modNameCopy.end(), modNameCopy.begin(), ::tolower);
			if (modNameCopy == nameCopy)
				return std::optional<std::shared_ptr<IModule>>(mod);
		}
		return std::optional<std::shared_ptr<IModule>>();
	}
};

extern ModuleManager* moduleMgr;
