#include "Jesus.h"

Jesus::Jesus() : IModule(0, MOVEMENT)
{
}


Jesus::~Jesus()
{
}

const char* Jesus::getModuleName()
{
	return "Jesus";
}

void Jesus::onTick(C_GameMode* gm)
{
	if (gm->player != nullptr) {
		
		if (gm->player->isSneaking()) {
			return;
		}
		else if (gm->player->hasEnteredWater()) {
			gm->player->velocity.y = 0.06f;
			gm->player->onGround = true;
			wasInWater = true;
		}
		else if (gm->player->isImmersedInWater()) {
			gm->player->velocity.y = 0.3f;
			gm->player->onGround = true;
			wasInWater = true;
		}
		else if (gm->player->isInWater() || gm->player->isInLava()) {
			gm->player->velocity.y = 0.1f;
			gm->player->onGround = true;
			wasInWater = true;
		}
		else if (wasInWater) {
			wasInWater = false;
			gm->player->velocity.x *= 1.2f;
			gm->player->velocity.x *= 1.2f;
		}
	}
}
