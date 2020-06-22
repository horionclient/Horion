#pragma once

#include "JoeGoal.h"
#include "JoeGoalY.h"
#include "JoeGoalXZ.h"

class JoeGoalXYZ : public JoeGoal {
private:
	vec3_ti targetPos;
public:
	JoeGoalXYZ(const vec3_ti& targetPos);

	bool isInGoal(vec3_ti pos) override;
	float getHeuristicEstimation(vec3_ti pos) override;
};
