#include "Jesus.h"

Jesus::Jesus() : IModule(0, Category::MOVEMENT, "Walk over water, like Jesus") {
}

Jesus::~Jesus() {
}

const char* Jesus::getModuleName() {
	return "Jesus";
}

void Jesus::onMove(C_MoveInputHandler* input) {
	isSneaking = input->isSneakDown;
}

void Jesus::onTick(C_GameMode* gm) {
	if (isSneaking && !gm->player->isInLava() || !gm->player->isOverWater() && gm->player->isInWater()) return;

	vec3_t pos(*gm->player->getPos());
	float ofs = 1.620010f;
	
	if (gm->player->hasEnteredWater() || gm->player->isInLava()) {
		pos.y = floorf(pos.y - ofs) + 0.6f + ofs;
		gm->player->setPos(pos);

		if (gm->player->velocity.y < 0) gm->player->velocity.y = 0;
		gm->player->fallDistance = 0;
		gm->player->onGround = true;
		gm->player->didEnterWaterBool = false;
	}
}
