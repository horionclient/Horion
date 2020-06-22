#pragma once

#include "../../../Utils/HMath.h"

class JoeGoal {
private:
public:
	virtual ~JoeGoal();

	virtual bool isInGoal(vec3_ti pos) = 0;
	virtual float getHeuristicEstimation(vec3_ti pos) = 0;
};
