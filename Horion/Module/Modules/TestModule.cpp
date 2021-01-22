#include "TestModule.h"
#include "../../../Utils/Logger.h"



TestModule::TestModule() : IModule(0, Category::MISC, "For testing purposes") {
	registerFloatSetting("PlayerSize", &this->playersize, this->playersize, 0, 10);
}

TestModule::~TestModule() {
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

void TestModule::onTick(C_GameMode* gm) {
	if (this->active) {
		g_Data.getLocalPlayer()->setSize(this->playersize / 2, this->playersize);
	}
}

void TestModule::onEnable() {
	this->ogsize = g_Data.getLocalPlayer()->height;
	this->active = true;
}

void TestModule::onDisable() {
	this->active = false;
	g_Data.getLocalPlayer()->setSize(this->ogsize / 2, this->ogsize);
}