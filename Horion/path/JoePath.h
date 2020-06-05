#pragma once

#include <vector>
#include "JoeSegment.h"

class JoePath {
private:
	std::vector<JoeSegment> segments;
public:
	JoePath(const std::vector<JoeSegment>& segments);
	JoePath();

	void draw() const;
};
