#include "Jesus.h"

Jesus::Jesus() : IModule(0, Category::MOVEMENT, "Walk over water, like Jesus") {
}

Jesus::~Jesus() {
}

const char* Jesus::getModuleName() {
	return "Jesus";
}

void Jesus::onLevelRender() {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	if (player->isSneaking()) return;

	if (player->hasEnteredWater()) {
		player->velocity.y = 0.06f;
		player->onGround = true;
		wasInWater = true;
	} else if (player->isInWater() || player->isInLava()) {
		player->velocity.y = 0.1f;
		player->onGround = true;
		wasInWater = true;
	} else {
		if (wasInWater) {
			wasInWater = false;
			player->velocity.x *= 1.2f;
			player->velocity.x *= 1.2f;
		}
	}
}
