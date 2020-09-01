#pragma once

#include <functional>

#include "Horion/Module/Modules/Module.h"
#include "Horion/path/JoeMovementController.h"
#include "Horion/path/JoePathFinder.h"
#include "Horion/path/goals/JoeGoal.h"

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
