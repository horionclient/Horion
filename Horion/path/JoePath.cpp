#include "JoePath.h"
#include "../DrawUtils.h"
JoePath::JoePath(const std::vector<JoeSegment>& segments) : segments(segments) {}
JoePath::JoePath() : segments() {
}
void JoePath::draw(int highlight) const {
	DrawUtils::setColor(13 / 255.f, 29 / 255.f, 48 / 255.f, 1);
	bool changedColor = false;
	for(int i = 0; i < segments.size(); i++){
		auto seg = segments[i];

		if(i == highlight){
			DrawUtils::setColor(39 / 255.f, 87 / 255.f, 144 / 255.f, 1);
			changedColor = true;
		}else if(changedColor)
			DrawUtils::setColor(13 / 255.f, 29 / 255.f, 48 / 255.f, 1);
		seg.draw();
	}
}
