#pragma once

#include "JoeGoal.h"

class JoeGoalY : public JoeGoal {
private:
	float target;
public:
	JoeGoalY(float target);

	bool isInGoal(vec3_ti pos) override;
	float getHeuristicEstimation(vec3_ti pos) override;

	static float heuristicEstimation(vec3_ti node, float target);
};
