#pragma once

#include "Module.h"
#include "../../path/JoePathFinder.h"
#include "../../path/JoeMovementController.h"
#include "../../path/goals/JoeGoal.h"

class FollowPathModule : public IModule {
private:
	std::shared_ptr<JoePathFinder> pathFinder;
	std::unique_ptr<JoeMovementController> movementController;
	std::shared_ptr<JoePath> path;
public:
	std::unique_ptr<JoeGoal> goal;

	FollowPathModule();

	const char *getModuleName() override;
	void onTick(C_GameMode *mode) override;
	void onEnable() override;
	void onDisable() override;
	void onLevelRender() override;
	void onMove(C_MoveInputHandler *handler) override;
};
