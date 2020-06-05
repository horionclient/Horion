#include "JoePath.h"
#include "../DrawUtils.h"
JoePath::JoePath(const std::vector<JoeSegment>& segments) : segments(segments) {}
JoePath::JoePath() : segments() {
}
void JoePath::draw() const {
	DrawUtils::setColor(13 / 255.f, 29 / 255.f, 48 / 255.f, 1);
	for(auto seg : segments){
		seg.draw();
	}
}
