#include "ModuleManager.h"

ModuleManager::ModuleManager(GameData* gameData) {
	this->gameData = gameData;
}

ModuleManager::~ModuleManager() {
	initialized = false;
	auto lock = this->lockModuleListExclusive();
	this->moduleList.clear();
}

void ModuleManager::initModules() {
	logF("Initializing modules");
	{
		auto lock = this->lockModuleListExclusive();

		this->moduleList.push_back(std::shared_ptr<IModule>(new HudModule()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Killaura()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new ESP()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Jetpack()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Aimbot()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new TriggerBot()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new ChestESP()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new NoKnockBack()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new RainbowSky()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new AirJump()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Step()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Glide()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new EditionFaker()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Phase()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Freecam()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new AutoSprint()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Tracer()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new BowAimbot()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Scaffold()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new NoFall()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Blink()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Nuker()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new InstaBreak()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new NoPacket()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Speed()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Xray()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new BedFucker()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new NoSlowDown()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new AutoTotem()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new InfiniteReach()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new ClickGuiMod()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new ChestStealer()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Fly()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Hitbox()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Reach()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new FullBright()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new AirSwim()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new InventoryMove()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new HighJump()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new NoWeb()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new FastLadder()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new StackableItem()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new FastEat()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Jesus()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new AutoArmor()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new InfiniteBlockReach()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new AutoClicker()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new NameTags()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Criticals()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Bhop()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Tower()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new BugUp()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new MidClick()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new ClickTP()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new NoFriends()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Spammer()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new ChestAura()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new AntiBot()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new InventoryCleaner()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Derp()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Crasher()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new CrystalAura()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Timer()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new NightMode()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new NoSwing()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new CubeGlide()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new AirStuck()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Zoom()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Teams()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Nbt()));

#if defined(_BETA) or defined(_DEBUG)
		this->moduleList.push_back(std::shared_ptr<IModule>(new ForceOpenCommandBlock()));
#endif

#ifdef _DEBUG
		this->moduleList.push_back(std::shared_ptr<IModule>(new PacketLogger()));

		this->moduleList.push_back(std::shared_ptr<IModule>(new TestModule()));
#endif

		// Sort module alphabetically
		std::sort(moduleList.begin(), moduleList.end(), [](auto lhs, auto rhs) {
			auto current = lhs;
			auto other = rhs;
			return std::string{*current->getModuleName()} < std::string{*other->getModuleName()};
		});

		initialized = true;
	}
	
	this->getModule<HudModule>()->setEnabled(true);
	this->getModule<ClickGuiMod>()->setEnabled(false);
	this->getModule<AntiBot>()->setEnabled(true);
}

void ModuleManager::disable() {
	auto lock = this->lockModuleList();
	for (std::vector<std::shared_ptr<IModule>>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		auto mod = *it;
		if (mod->isEnabled())
			mod->setEnabled(false);
	}
}

void ModuleManager::onLoadConfig(json* conf) {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (std::vector<std::shared_ptr<IModule>>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		auto mod = *it;
		mod->onLoadConfig(conf);
	}

	this->getModule<HudModule>()->setEnabled(true);
	this->getModule<ClickGuiMod>()->setEnabled(false);
	this->getModule<AntiBot>()->setEnabled(true);
}

void ModuleManager::onSaveConfig(json* conf) {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (std::vector<std::shared_ptr<IModule>>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		auto mod = *it;
		mod->onSaveConfig(conf);
	}
}

void ModuleManager::onTick(C_GameMode* gameMode) {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (std::vector<std::shared_ptr<IModule>>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		auto mod = *it;
		if (mod->isEnabled())
			mod->onTick(gameMode);
	}
}

void ModuleManager::onKeyUpdate(int key, bool isDown) {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (std::vector<std::shared_ptr<IModule>>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		auto mod = *it;
		mod->onKeyUpdate(key, isDown);
	}
}

void ModuleManager::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized())
		return;
	auto mutex = this->lockModuleList();

	for (std::vector<std::shared_ptr<IModule>>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		auto mod = *it;
		if (mod->isEnabled())
			mod->onPreRender(renderCtx);
	}
}

void ModuleManager::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized())
		return;
	auto mutex = this->lockModuleList();

	for (std::vector<std::shared_ptr<IModule>>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		auto mod = *it;
		if (mod->isEnabled())
			mod->onPostRender(renderCtx);
	}
}

void ModuleManager::onSendPacket(C_Packet* packet) {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto it : moduleList) {
		if (it->isEnabled())
			it->onSendPacket(packet);
	}
}

std::vector<std::shared_ptr<IModule>>* ModuleManager::getModuleList() {
	return &this->moduleList;
}

int ModuleManager::getModuleCount() {
	return (int)moduleList.size();
}

int ModuleManager::getEnabledModuleCount() {
	int i = 0;
	auto lock = this->lockModuleList();
	for (auto it = (&moduleList)->begin(); it != (&moduleList)->end(); ++it) {
		if ((*it)->isEnabled()) i++;
	}
	return i;
}

ModuleManager* moduleMgr = new ModuleManager(&g_Data);
