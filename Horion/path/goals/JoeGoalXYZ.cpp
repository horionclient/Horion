#include "JoeGoalXYZ.h"
bool JoeGoalXYZ::isInGoal(vec3_ti pos) {
	return pos == targetPos;
}
float JoeGoalXYZ::getHeuristicEstimation(vec3_ti pos) {
	return JoeGoalXZ::heuristicEstimation(pos, targetPos) + JoeGoalY::heuristicEstimation(pos, (float)targetPos.y);
}
JoeGoalXYZ::JoeGoalXYZ(const vec3_ti& targetPos) : targetPos(targetPos) {}
