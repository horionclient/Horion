#include "JoeSegment.h"

#include "../DrawUtils.h"

JoeSegment::JoeSegment(JoeSegmentType type, vec3_ti& start, vec3_ti& stop, float cost, bool allowSprint) : segmentType(type), start(start), end(stop), cost(cost), allowSprint(allowSprint) {

}
void JoeSegment::draw() {
	//for(auto p : this->validPositions){
	//	DrawUtils::drawLine3d(p.toFloatVector(), p.add(1, 1, 1).toFloatVector());
	//}

	switch (segmentType) {
		case DROP: {
			auto dropPoint = start.toVec3t().add(0.5f, 0.05f, 0.5f);
			dropPoint.x = end.x + 0.5f;
			dropPoint.z = end.z + 0.5f;
			DrawUtils::drawLine3d(start.toVec3t().add(0.5f, 0.05f, 0.5f), dropPoint);
			DrawUtils::drawLine3d(dropPoint, end.toVec3t().add(0.5f, 0.05f, 0.5f));
		} break;
		case JUMP:
			DrawUtils::drawLine3d(start.toVec3t().add(0.5f, 0.05f, 0.5f), start.toVec3t().add(0.5f, 1.05f, 0.5f));
			DrawUtils::drawLine3d(start.toVec3t().add(0.5f, 1.05f, 0.5f), end.toVec3t().add(0.5f, 0.05f, 0.5f));
			break;
		case PARKOUR_JUMP_SINGLE:{
			auto middle = start.toVec3t().add(0.5f, 0.05f, 0.5f).add(end.sub(start).toVec3t().mul(0.5f).add(0, 1, 0));
			DrawUtils::drawLine3d(start.toVec3t().add(0.5f, 0.05f, 0.5f), middle);
			DrawUtils::drawLine3d(middle, end.toVec3t().add(0.5f, 0.05f, 0.5f));
		} break;
		case WALK:
		default:
			DrawUtils::drawLine3d(start.toVec3t().add(0.5f, 0.05f, 0.5f), end.toVec3t().add(0.5f, 0.05f, 0.5f));
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
float JoeSegment::getCost() const {
	return cost;
}
bool JoeSegment::isAllowingSprint() const {
	return allowSprint;
}
void JoeSegment::setAllowSprint(bool allowSprint) {
	JoeSegment::allowSprint = allowSprint;
}
bool JoeSegment::isInValidPosition(const vec3_ti& pos) const {
	for(const auto& validPos : this->validPositions){
		if(pos == validPos)
			return true;
	}
	return false;
}
void JoeSegment::setValidPositions(const std::vector<vec3_ti>& validPositions) {
	JoeSegment::validPositions = validPositions;
}
void JoeSegment::init() {
	std::vector<vec3_ti> positions = {start, end};

	auto player = g_Data.getLocalPlayer();
	auto reg = player->region;

	switch(this->segmentType){
		case JoeSegmentType::WALK: {
			if(start.y == end.y){
				int isDiagonal = 0;
				isDiagonal += start.x != end.x;
				isDiagonal += start.z != end.z;
				if(!isDiagonal){
					break;
				}

				auto mod = start;
				mod.x = end.x;
				positions.push_back(mod);
				mod.x = start.x;
				mod.z = end.z;
				positions.push_back(mod);
				break;
			}

			vec3_t tangentF = end.sub(start).toFloatVector();
			tangentF.y = 0;
			tangentF = tangentF.normalize();
			vec3_ti tangent((int)roundf(tangentF.x), 0, (int)roundf(tangentF.z));

			positions.push_back(start.add(tangent));
			positions.push_back(start.add(tangent).add(0, -1, 0));

			for(int yLevel = start.y; yLevel > end.y; yLevel--){
				auto mod = end;
				mod.y = yLevel;
				positions.push_back(mod);
			}
		} break;
		case JoeSegmentType::DROP: {
			for(int yLevel = start.y; yLevel > end.y; yLevel--){
				auto mod = end;
				mod.y = yLevel;
				positions.push_back(mod);
			}
			auto pos = end;
			auto block = reg->getBlock(pos);
			while(block->toLegacy()->hasWater(reg, pos) && pos.y > 1 && end.y - pos.y < 19){
				positions.push_back(pos);
				pos.y--;
			}
		} break;
		case JoeSegmentType::JUMP: {
			auto mod = start;
			mod.y = end.y;
			positions.push_back(mod);
		} break;
		case JoeSegmentType::PARKOUR_JUMP_SINGLE: {
			vec3_t tangentF = end.sub(start).toFloatVector();
			tangentF.y = 0;
			tangentF = tangentF.normalize();
			vec3_ti tangent((int)roundf(tangentF.x), 0, (int)roundf(tangentF.z));

			positions.push_back(start.add(0, 1, 0));

			auto mod = start.add(tangent);
			positions.push_back(mod);
			positions.push_back(mod.add(0, 1, 0));

			mod = end;
			for(int yLevel = end.y; yLevel <= start.y + 1; yLevel++){
				mod.y = yLevel;
				positions.push_back(mod);
			}
		} break;
		case JoeSegmentType::WATER_WALK: {
			positions.push_back(start.add(0, 1, 0));
			if(end.y > start.y)
				positions.push_back(start.add(0, 2, 0));
			positions.push_back(end.add(0, 1, 0));
		} break;
		default: {

		} break;
	}
	this->setValidPositions(positions);
}
