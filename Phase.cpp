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

    if (gm->player->isSneaking() && !hasYiked) {
		hasYiked = !hasYiked;
        gm->player->setPos(vec3_t(gm->player->getPos()->x, gm->player->getPos()->y - 1, gm->player->getPos()->z));
    }

	if (!gm->player->isSneaking()) {
		hasYiked = false;
	}

    if (gm->player->isJumping() && !hasYot) {
		hasYot = !hasYot;
        gm->player->setPos(vec3_t(gm->player->getPos()->x, gm->player->getPos()->y + 1, gm->player->getPos()->z));
    }

	if (!gm->player->isJumping()) {
		hasYot = false;
	}
}
void Phase::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr)
		g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
}
