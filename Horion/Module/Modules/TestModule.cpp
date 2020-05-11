#include "TestModule.h"

TestModule::TestModule() : IModule(0, Category::EXPLOITS, "For testing purposes") {
}

TestModule::~TestModule() {
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

bool TestModule::isFlashMode() {
	return false;
}

void TestModule::onEnable() {
}

void TestModule::onTick(C_GameMode* gm) {
}

void TestModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
}

void TestModule::onDisable() {
}
