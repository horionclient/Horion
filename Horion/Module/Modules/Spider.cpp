#include "Spider.h"

Spider::Spider() : IModule('Z', Category::MOVEMENT, "Climb walls") {
	registerFloatSetting("Speed", &speed, speed, 0.1, 1);
}

Spider::~Spider() {
}

const char* Spider::getModuleName() {
	return "Spider";
}

void Spider::onMove(C_MoveInputHandler* input) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;

	if (player->isInLava() || player->isInWater())
		return;

	if (player->isSneaking())
		return;

	if (!(input->forward || input->backward) || (input->forward && input->backward))
		return;
	
	static std::vector<vec3_ti*> sideBlocks;
	if (sideBlocks.empty()) {
		sideBlocks.push_back(new vec3_ti(0, 0, -1));
		sideBlocks.push_back(new vec3_ti(0, 0, 1));
		sideBlocks.push_back(new vec3_ti(-1, 0, 0));
		sideBlocks.push_back(new vec3_ti(1, 0, 0));
	}

	for (auto current : sideBlocks) {
		vec3_ti side = vec3_ti(player->currentPos.floor()).add(*current);
		C_BlockLegacy* sideBlock = *(player->region->getBlock(side)->blockLegacy);
		AABB fullBlock = AABB(side.toFloatVector(), side.add(1, 1, 1).toFloatVector()); // todo: idk why all blocks have no AABB
		if (sideBlock->material->isBlockingMotion && fullBlock.intersects(player->aabb.expanded(0.25))) {
			player->velocity.y = speed;
			return;
		}
	}
}