#include "TestModule.h"
#include "../../../Utils/Logger.h"
#include "../../DrawUtils.h"

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

std::vector<vec3_ti> lastPos;
void TestModule::onEnable() {
	lastPos.clear();
}

void TestModule::onTick(C_GameMode* gm) {
	/*auto pPos = gm->player->eyePos0;
	vec3_ti startNode((int)floorf(pPos.x), (int)roundf(pPos.y - 1.62f), (int)floorf(pPos.z));

	if(std::find(lastPos.begin(), lastPos.end(), startNode) == lastPos.end()){
		lastPos.push_back(startNode);
	}*/
	logF("%.2f", gm->player->velocity.z);
}

void TestModule::onMove(C_MoveInputHandler* hand){

}

void TestModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	for(auto pos : lastPos){
		DrawUtils::drawBox(pos.toFloatVector(), pos.add(1, 1, 1).toFloatVector(), 1, false);
	}
}

void TestModule::onSendPacket(C_Packet* p) {
}

void TestModule::onDisable() {

}
