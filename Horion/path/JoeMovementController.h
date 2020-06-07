#pragma once

#include "JoePath.h"
#include "../../SDK/CEntity.h"
#include "../../SDK/CMoveInputHandler.h"

class JoeMovementController {
private:
	JoePath currentPath;
	int currentPathSegment = 0;
public:
	bool overrideViewAngles = false;
	vec2_t targetViewAngles = {0, 0};

	JoeMovementController(JoePath path);

	void step(C_LocalPlayer* player, C_MoveInputHandler* movementHandler);
};
