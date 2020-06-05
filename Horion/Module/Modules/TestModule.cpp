#include "TestModule.h"
#include "../../../Utils/Logger.h"
#include "../../path/JoePathFinder.h"

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

bool found = false;
JoePathFinder* pathFinder = nullptr;
JoePath path;

void TestModule::onEnable() {
	if(!g_Data.isInGame() || !g_Data.getLocalPlayer()->isAlive()){
		setEnabled(false);
		return;
	}
	found = false;

	auto player = g_Data.getLocalPlayer();
	auto pPos = player->eyePos0;
	vec3_ti startNode((int)floorf(pPos.x), (int)roundf(pPos.y - 1.62f), (int)floorf(pPos.z));
	logF("Start node: %i %i %i", startNode.x, startNode.y, startNode.z);
	pathFinder = new JoePathFinder(startNode, player->region);
	std::thread([&](){
	  path = pathFinder->findPathTo(vec3_ti(-2, 4, 2));
	  found = true;
	  Sleep(100);
	  logF("Done");
	  delete pathFinder;
	}).detach();
}


void TestModule::onTick(C_GameMode* gm) {
	//if(gm->player->onGround)
	//	logF("yeet");
}

void TestModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if(pathFinder && !found){
		JoePath localPath = pathFinder->getCurrentPath();
		localPath.draw(); // copy so we avoid drawing while its being updated by the pathfinder
	}else if(found){
		JoePath localPath = path;
		localPath.draw();
	}

}

void TestModule::onSendPacket(C_Packet* p) {
}

void TestModule::onDisable() {
}
