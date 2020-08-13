#pragma once

#include <vector>
#include "JoeSegment.h"

class JoePath {
private:
	std::vector<JoeSegment> segments;
	bool isIncomplete;
	bool isInitialized = 0;
public:
	JoePath(const std::vector<JoeSegment>& segments, bool isIncomplete);
	JoePath();

	void cutoff(float percentageKeep);

	auto& getAllSegments(){
		return segments;
	}
	size_t getNumSegments(){
		return this->segments.size();
	}
	auto getSegment(size_t seg){
		return this->segments[seg];
	}

	bool isInitialized1() const;
	void initPathSegments();
	bool isIncomplete1() const;
	void draw(int) const;
};
