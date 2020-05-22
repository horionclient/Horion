#include "TestModule.h"

TestModule::TestModule() : IModule(0, Category::MISC, "For testing purposes") {
}

TestModule::~TestModule() {
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

bool TestModule::isFlashMode() {
	return true;
}

void TestModule::onEnable() {
}

void TestModule::onTick(C_GameMode* gm) {
}

void TestModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
}

void TestModule::onSendPacket(C_Packet* p) {
}

void TestModule::onDisable() {
	g_Data.getLocalPlayer()->velocity = vec3_t(0, 0, 0);
}
