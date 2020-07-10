#include "FollowPathModule.h"

#include "../../../Utils/Logger.h"

FollowPathModule::FollowPathModule() : IModule(0, Category::MOVEMENT, "Follows joe paths") {}

const char *FollowPathModule::getModuleName() {
	return "FollowPath";
}

void FollowPathModule::startSearch(vec3_ti startNode, C_BlockSource* region, float searchTimeout, std::function<void(bool, JoePath)> callback){
	if(this->pathFinder){
		logF("Already searching!");
		return;
	}
	this->pathFinder = std::make_shared<JoePathFinder>(startNode, region, this->goal);
	this->pathFinder->pathSearchTimeout = searchTimeout;
	std::thread([this, callback](){
		auto ref = this->pathFinder; // so it won't get deleted when followpathmodule is disabled
		auto tempPath = this->pathFinder->findPath();
		this->pathFinder.reset();
		if(tempPath.getNumSegments() == 0 || !this->isEnabled()){
			callback(false, tempPath);
			return;
		}
		callback(true, tempPath);
	}).detach();
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

	this->startSearch(startNode, player->region, 3, [&](bool succeeded, JoePath tempPath){
		if(!succeeded){
			this->path.reset();
			this->movementController.reset();
			this->setEnabled(false);
			this->clientMessageF("%sCould not find a path!", RED);
			this->engageDelay = -1;
			return;
		}

		this->clientMessageF("%sFound %s path!", tempPath.isIncomplete1() ? YELLOW : GREEN, tempPath.isIncomplete1() ? "incomplete" : "complete");

		if(tempPath.isIncomplete1()){
			tempPath.cutoff(0.9f);
		}
		this->engageDelay = 10;

		this->path = std::make_shared<JoePath>(tempPath.getAllSegments(), tempPath.isIncomplete1());
		this->movementController = std::make_unique<JoeMovementController>(path);
	});
}

void FollowPathModule::onDisable() {
	if(this->pathFinder)
		this->pathFinder->terminateSearch = true;
	this->engageDelay = -1;

	this->pathFinder.reset();
	this->path.reset();
	this->nextPath.reset();
	this->movementController.reset();
	this->goal.reset();
}

void FollowPathModule::onTick(C_GameMode *mode) {

}

void FollowPathModule::onMove(C_MoveInputHandler *handler) {
	if(this->movementController){
		this->movementController->step(g_Data.getLocalPlayer(), g_Data.getClientInstance()->getMoveTurnInput());
		if(this->engageDelay > 0)
			this->engageDelay--;

		if(this->movementController->isDone()){
			if(this->movementController->getCurrentPath()->isIncomplete1()){
				// Replace with next path if it exists
				if(this->nextPath && !pathFinder){
					this->clientMessageF("%sContinuing on next path...", GREEN);

					this->path = this->nextPath;
					this->nextPath.reset();
					this->movementController = std::make_unique<JoeMovementController>(path);
				}else if(!pathFinder){
					this->setEnabled(false);
				}else if(g_Data.getLocalPlayer()->isInWater()){
					handler->autoJumpInWater = true;
				}
			}else{
				this->clientMessageF("%sDone!", GREEN);
				this->setEnabled(false);
				return;
			}
		}else if(!this->pathFinder && this->engageDelay == 0 && this->path && this->path->isIncomplete1() && !this->nextPath){
			this->engageDelay = 10;

			// Estimate time to completion
			auto curPath = this->movementController->getCurrentPath();
			float timeSpent = 0;
			if(curPath->getNumSegments() == 0){
				this->setEnabled(false);
				return;
			}
			for(size_t i = curPath->getNumSegments() - 1; i > this->movementController->getCurrentPathSegment(); i--){
				auto cur = curPath->getSegment(i);
				timeSpent += cur.getCost();
				if(timeSpent > 11)
					break;
			}

			if(timeSpent > 11)
				return;

			this->clientMessageF("%sCalculating next path...", YELLOW);

			float timeForSearch = std::clamp(timeSpent - 0.5f, 5.f, 10.f);
			auto lastSeg = curPath->getSegment(curPath->getNumSegments() - 1);
			this->nextPath.reset();
			this->startSearch(lastSeg.getEnd(), g_Data.getLocalPlayer()->region, timeForSearch, [&](bool succeeded, JoePath tempPath){
			  if(!succeeded){
				  this->clientMessageF("%sCould not find subsequent path!", RED);

				  this->engageDelay = -1;
				  return;
			  }

			  this->clientMessageF("%sFound subsequent %s path!", tempPath.isIncomplete1() ? YELLOW : GREEN, tempPath.isIncomplete1() ? "incomplete" : "complete");

			  if(tempPath.isIncomplete1()){
				  tempPath.cutoff(0.9f);
			  }

			  this->nextPath = std::make_shared<JoePath>(tempPath.getAllSegments(), tempPath.isIncomplete1());
			});
		}
	}
}
void FollowPathModule::onLevelRender() {
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
