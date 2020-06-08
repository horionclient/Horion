#include "JoeGoalY.h"

#include "../JoeConstants.h"

JoeGoalY::JoeGoalY(float target) : target(target) {}
bool JoeGoalY::isInGoal(vec3_ti pos) {
	return pos.y == target;
}
float JoeGoalY::getHeuristicEstimation(vec3_ti pos) {
	return heuristicEstimation(pos, target);
}
float JoeGoalY::heuristicEstimation(vec3_ti node, float target) {
	if(node.y > target)
		return 0.99f * DROP3_TIME * ((node.y - target) / 3);
	if(target > node.y)
		return (target - node.y) * JUMP_TIME * 0.99f;
	return 0;
}
