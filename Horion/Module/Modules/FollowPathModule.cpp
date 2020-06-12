#include "FollowPathModule.h"

#include "../../../Utils/Logger.h"

FollowPathModule::FollowPathModule() : IModule(0, Category::MOVEMENT, "Follows joe paths") {}

const char *FollowPathModule::getModuleName() {
#ifdef _DEBUG
	return "FollowPath";
#else
	return "Joe";
#endif
}

void FollowPathModule::onEnable() {
	if(!g_Data.isInGame() || !g_Data.getLocalPlayer()->isAlive()){
		setEnabled(false);
		return;
	}

	if(!this->goal){
		logF("goal not set");
		setEnabled(false);
		return;
	}

	auto player = g_Data.getLocalPlayer();
	auto pPos = player->eyePos0;
	vec3_ti startNode((int)floorf(pPos.x), (int)roundf(pPos.y - 1.62f), (int)floorf(pPos.z));
	logF("Start node: %i %i %i", startNode.x, startNode.y, startNode.z);

	this->pathFinder = std::make_shared<JoePathFinder>(startNode, player->region, std::move(this->goal));
	this->pathFinder->pathSearchTimeout = 3;
	std::thread([&](){
		auto ref = this->pathFinder; // so it won't get deleted when followpathmodule is disabled
		auto tempPath = pathFinder->findPath();
		if(tempPath.getNumSegments() == 0 || !this->isEnabled()){
			this->path.reset();
			this->movementController.reset();
			this->setEnabled(false);
			if(g_Data.getGuiData() != nullptr)
				g_Data.getGuiData()->displayClientMessageF("%sCould not find a path!", RED);
			return;
		}

		if(g_Data.getGuiData() != nullptr)
			g_Data.getGuiData()->displayClientMessageF("%sFound %s path!", tempPath.isIncomplete1() ? YELLOW : GREEN, tempPath.isIncomplete1() ? "incomplete" : "complete");
		if(tempPath.isIncomplete1()){
			tempPath.cutoff(0.9f);
		}

		this->path = std::make_shared<JoePath>(tempPath.getAllSegments(), tempPath.isIncomplete1());
		this->movementController = std::make_unique<JoeMovementController>(path);
	}).detach();
}

void FollowPathModule::onDisable() {
	if(this->pathFinder)
		this->pathFinder->terminateSearch = true;

	this->pathFinder.reset();
	this->path.reset();
	this->movementController.reset();
	this->goal.reset();
}

void FollowPathModule::onTick(C_GameMode *mode) {

}

void FollowPathModule::onPostRender(C_MinecraftUIRenderContext *renderCtx) {
	if(!g_Data.isInGame()){
		this->setEnabled(false);
		return;
	}

	if(this->movementController && this->path){
		this->path->draw(this->movementController->getCurrentPathSegment());
	}else if(this->pathFinder){
		JoePath localPath = this->pathFinder->getCurrentPath();
		localPath.draw(-1); // copy so we avoid drawing while its being updated by the pathfinder
	}
}
void FollowPathModule::onMove(C_MoveInputHandler *handler) {
	if(this->movementController){
		this->movementController->step(g_Data.getLocalPlayer(), g_Data.getClientInstance()->getMoveTurnInput());
		if(this->movementController->isDone()){
			this->setEnabled(false);
		}
	}
}
