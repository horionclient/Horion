#pragma once

#include "../../Utils/HMath.h"

enum JoeSegmentType{
	INVALID = -1,
	WALK = 0,
	JUMP,
	DROP,
	PARKOUR_JUMP_SINGLE,
	WATER_WALK
};

class JoeSegment {
private:
	JoeSegmentType segmentType;
	vec3_ti start, end;
	float cost;
public:
	JoeSegment(JoeSegmentType type, vec3_ti& start, vec3_ti& stop, float cost);
	void draw();
	JoeSegmentType getSegmentType() const;
	const vec3_ti& getStart() const;
	const vec3_ti& getEnd() const;
	float getCost() const;
};
