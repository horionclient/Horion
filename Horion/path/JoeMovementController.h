#pragma once

#include "JoePath.h"
#include "../../SDK/CEntity.h"
#include "../../SDK/CMoveInputHandler.h"

class JoeMovementController {
private:
	std::shared_ptr<JoePath> currentPath;
	int currentPathSegment = 0;
public:
	bool overrideViewAngles = false;
	vec2_t targetViewAngles = {0, 0};

	JoeMovementController(std::shared_ptr<JoePath> path);

	void step(C_LocalPlayer* player, C_MoveInputHandler* movementHandler);
	bool isDone(){
		return currentPathSegment >= currentPath->getNumSegments();
	}
};
