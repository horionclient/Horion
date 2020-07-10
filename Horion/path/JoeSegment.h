#pragma once

#include "../../Utils/HMath.h"
#include <vector>

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
	bool allowSprint;
	std::vector<vec3_ti> validPositions;
public:
	JoeSegment(JoeSegmentType type, vec3_ti& start, vec3_ti& stop, float cost, bool allowSprint = false);
	void draw();
	bool isAllowingSprint() const;
	void setAllowSprint(bool allowSprint);
	JoeSegmentType getSegmentType() const;
	const vec3_ti& getStart() const;
	const vec3_ti& getEnd() const;
	bool isInValidPosition(const vec3_ti& pos) const;
	void setValidPositions(const std::vector<vec3_ti>& validPositions);
	float getCost() const;
	void init();
};
