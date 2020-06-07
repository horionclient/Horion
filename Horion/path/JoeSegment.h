#pragma once

#include "../../Utils/HMath.h"

enum JoeSegmentType{
	INVALID = -1,
	WALK = 0,
	JUMP,
	DROP
};

class JoeSegment {
private:
	JoeSegmentType segmentType;
	vec3_ti start, end;
public:
	JoeSegment(JoeSegmentType type, vec3_ti& start, vec3_ti& stop);
	void draw();
	JoeSegmentType getSegmentType() const;
	const vec3_ti& getStart() const;
	const vec3_ti& getEnd() const;
};
