#include "Phase.h"



Phase::Phase() : IModule('I', Category::MOVEMENT, "Walk through walls")
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
}
void Phase::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr)
		g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
}