#include "JoeGoalXZ.h"

#include "../JoeConstants.h"

bool JoeGoalXZ::isInGoal(vec3_ti pos) {
	return pos.x == targetPos.x && pos.z == targetPos.z;
}
float JoeGoalXZ::getHeuristicEstimation(vec3_ti pos) {
	return JoeGoalXZ::heuristicEstimation(targetPos, pos);
}
float JoeGoalXZ::heuristicEstimation(vec3_ti node, vec3_ti target) {
	vec3_ti diff = node.sub(target);
	int x = abs(diff.x);
	int z = abs(diff.z);
	float straight;
	float diagonal;
	if (x < z) {
		straight = (float)z - x;
		diagonal = (float)x;
	} else {
		straight = (float)x - z;
		diagonal = (float)z;
	}
	static const float SQRT_2 = sqrtf(2);
	diagonal *= SQRT_2;
	return (straight + diagonal) / SPRINT_SPEED;
}
JoeGoalXZ::JoeGoalXZ(const vec3_ti& targetPos) : targetPos(targetPos) {}
