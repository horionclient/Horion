#pragma once

#include "Module.h"
#include "../../path/JoePathFinder.h"
#include "../../path/JoeMovementController.h"
#include "../../path/goals/JoeGoal.h"
#include <functional>

class FollowPathModule : public IModule {
private:
	std::shared_ptr<JoePathFinder> pathFinder;
	std::unique_ptr<JoeMovementController> movementController;
	std::shared_ptr<JoePath> path;
	std::shared_ptr<JoePath> nextPath;
	int engageDelay = -1;
public:
	std::shared_ptr<JoeGoal> goal;

	FollowPathModule();

	const char *getModuleName() override;
	void startSearch(vec3_ti startNode, C_BlockSource* region, float searchTimeout, std::function<void(bool, JoePath)> callback);
	void onTick(C_GameMode *mode) override;
	void onEnable() override;
	void onDisable() override;
	void onLevelRender() override;
	void onMove(C_MoveInputHandler *handler) override;
};
