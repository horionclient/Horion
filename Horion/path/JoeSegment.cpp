#include "JoeSegment.h"

#include "../DrawUtils.h"

JoeSegment::JoeSegment(JoeSegmentType type, vec3_ti& start, vec3_ti& stop) : segmentType(type), start(start), end(stop) {
}
void JoeSegment::draw() {
	switch (segmentType) {
		case DROP: {
			auto dropPoint = start.toVec3t().add(0.5f, 0, 0.5f);
			dropPoint.x = end.x + 0.5f;
			dropPoint.z = end.z + 0.5f;
			DrawUtils::drawLine3d(start.toVec3t().add(0.5f, 0, 0.5f), dropPoint);
			DrawUtils::drawLine3d(dropPoint, end.toVec3t().add(0.5f, 0, 0.5f));
		} break;
		case JUMP:
			DrawUtils::drawLine3d(start.toVec3t().add(0.5f, 0, 0.5f), start.toVec3t().add(0.5f, 1, 0.5f));
			DrawUtils::drawLine3d(start.toVec3t().add(0.5f, 1, 0.5f), end.toVec3t().add(0.5f, 0, 0.5f));
			break;
		case PARKOUR_JUMP_SINGLE:{
			auto middle = start.toVec3t().add(0.5f, 0, 0.5f).add(end.sub(start).toVec3t().mul(0.5f).add(0, 1, 0));
			DrawUtils::drawLine3d(start.toVec3t().add(0.5f, 0, 0.5f), middle);
			DrawUtils::drawLine3d(middle, end.toVec3t().add(0.5f, 0, 0.5f));
		} break;
		case WALK:
		default:
			DrawUtils::drawLine3d(start.toVec3t().add(0.5f, 0, 0.5f), end.toVec3t().add(0.5f, 0, 0.5f));
			break;
	}
}
JoeSegmentType JoeSegment::getSegmentType() const {
	return segmentType;
}
const vec3_ti& JoeSegment::getStart() const {
	return start;
}
const vec3_ti& JoeSegment::getEnd() const {
	return end;
}
