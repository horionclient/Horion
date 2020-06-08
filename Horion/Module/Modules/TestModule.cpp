#include "TestModule.h"
#include "../../../Utils/Logger.h"
#include "../../path/JoePathFinder.h"
#include "../../path/JoeMovementController.h"

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
std::shared_ptr<JoeMovementController> movementController;
JoePathFinder* pathFinder = nullptr;
JoePath path;

int delay = 0;
void TestModule::onEnable() {
	if(!g_Data.isInGame() || !g_Data.getLocalPlayer()->isAlive()){
		setEnabled(false);
		return;
	}
	delay = 0;

	found = false;

	auto player = g_Data.getLocalPlayer();
	auto pPos = player->eyePos0;
	vec3_ti startNode((int)floorf(pPos.x), (int)roundf(pPos.y - 1.62f), (int)floorf(pPos.z));
	logF("Start node: %i %i %i", startNode.x, startNode.y, startNode.z);

	pathFinder = new JoePathFinder(startNode, player->region);
	std::thread([&](){
	  path = pathFinder->findPathTo(vec3_ti(98, 5, 261));
	  movementController = std::make_unique<JoeMovementController>(path);
	  found = true;
	  Sleep(50);
	  logF("Done");
	  delete pathFinder;
	  pathFinder = nullptr;
	}).detach();
}

vec3_t lastPos = {0, 0, 0};

void TestModule::onTick(C_GameMode* gm) {
	//if(gm->player->onGround)
	//	logF("yeet");

	//logF("z:%.2f, %i", gm->player->eyePos0.z, gm->player->onGround);
	auto nowPos = gm->player->eyePos0;
	//vec3_ti bPos = nowPos.sub(vec3_t(0.f, 1.62f, 0.f));
	//auto block = gm->player->region->getBlock(bPos);

	delay++;
	if(delay == 15 * 20 && pathFinder && !found) // 10 sec
		pathFinder->terminateSearch = true;

	lastPos = gm->player->eyePos0;
}

void TestModule::onMove(C_MoveInputHandler* hand){
	if(found && movementController){
		movementController->step(g_Data.getLocalPlayer(), g_Data.getClientInstance()->getMoveTurnInput());
	}
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
	if(pathFinder && !found)
		pathFinder->terminateSearch = true;
}
