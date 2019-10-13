#include "Phase.h"



Phase::Phase() : IModule('I', MOVEMENT)
{
}


Phase::~Phase()
{
}

const char* Phase::getModuleName()
{
	return ("Phase");
}

void Phase::onTick(C_GameMode* gm) {
    if(gm->player != nullptr)
        gm->player->aabb.upper.y = gm->player->aabb.lower.y;

    if (gm->player->isSneaking) {
        gm->player->setPos(gm->player->getPos - vec3_t(0, 1, 0));
    }

    if (gm->player->isJumping) {
        gm->player->setPos(gm->player->getPos + vec3_t(0, 1, 0));
    }
}
void Phase::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr)
		g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
}
