#include "TestModule.h"
#include "../../../Utils/Logger.h"


TestModule::TestModule() : IModule(0, Category::MISC, "For testing purposes") {
}

TestModule::~TestModule() {
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

bool TestModule::isFlashMode() {
	return false;
}

int delay = 0;
void TestModule::onEnable() {
}


void TestModule::onTick(C_GameMode* gm) {
	if(!gm->player->onGround)
		logF("ssss");
}

void TestModule::onMove(C_MoveInputHandler* hand){

}

void TestModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {

}

void TestModule::onSendPacket(C_Packet* p) {
}

void TestModule::onDisable() {

}
