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



vec3_t lastPos{};
void TestModule::onTick(C_GameMode* gm) {

	auto diff = gm->player->eyePos0.sub(lastPos);
	diff.y = 0;
	auto player = g_Data.getLocalPlayer();
	auto pPos = player->eyePos0;
	vec3_ti startNode((int)floorf(pPos.x), (int)roundf(pPos.y - 1.62f), (int)floorf(pPos.z));
	auto block = player->region->getBlock(startNode);

	vec3_t flow{};
	block->toLegacy()->liquidGetFlow(&flow, player->region, &startNode);
	lastPos = gm->player->eyePos0;
}

void TestModule::onMove(C_MoveInputHandler* hand){

}

void TestModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {

}

void TestModule::onSendPacket(C_Packet* p) {
}

void TestModule::onDisable() {

}
