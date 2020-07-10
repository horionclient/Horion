#include "JoePath.h"
#include "../DrawUtils.h"
#include "../../Utils/Logger.h"
JoePath::JoePath(const std::vector<JoeSegment>& segments, bool isIncomplete) : segments(segments), isIncomplete(isIncomplete) {

}
JoePath::JoePath() : segments(), isIncomplete(false) {
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
bool JoePath::isIncomplete1() const {
	return isIncomplete;
}
void JoePath::cutoff(float percentageKeep) {
	if(this->getNumSegments() == 0)
		return;
	int numKeep = (int)ceilf(this->getNumSegments() * percentageKeep);
	this->segments.erase(this->segments.begin() + numKeep, this->segments.end());
}
void JoePath::initPathSegments() {
	// Init segments
	for(auto& seg : segments){
		seg.init();
	}

	// Check whether we can sprint or not
	if(segments.size() > 1){

		for(int i = 0; i < segments.size() - 2; i++){
			auto& curSeg = segments.at(i);
			auto& nextSeg = segments.at(i + 1);
			if(curSeg.getSegmentType() != JoeSegmentType::WALK || nextSeg.getSegmentType() != JoeSegmentType::WALK){
				curSeg.setAllowSprint(false);
				continue;
			}
			if(curSeg.getStart().y != curSeg.getEnd().y){
				curSeg.setAllowSprint(false);
				continue;
			}

			auto currentTangent = curSeg.getEnd().sub(curSeg.getStart()).toFloatVector().normalize();
			auto nextTangent = nextSeg.getEnd().sub(nextSeg.getStart()).toFloatVector().normalize();
			if(currentTangent.dot(nextTangent) > 0.3f){ // make sure we're running in a similar direction
				curSeg.setAllowSprint(true);
			}
		}
	}

	this->isInitialized = true;
}
bool JoePath::isInitialized1() const {
	return isInitialized;
}
