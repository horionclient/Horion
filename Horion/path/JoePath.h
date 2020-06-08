#pragma once

#include <vector>
#include "JoeSegment.h"

class JoePath {
private:
	std::vector<JoeSegment> segments;
public:
	JoePath(const std::vector<JoeSegment>& segments);
	JoePath();

	auto& getAllSegments(){
		return segments;
	}
	size_t getNumSegments(){
		return this->segments.size();
	}
	auto getSegment(size_t seg){
		return this->segments[seg];
	}
	void draw(int) const;
};
