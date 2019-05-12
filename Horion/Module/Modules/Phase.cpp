#include "Phase.h"



Phase::Phase() : IModule('I')
{
}


Phase::~Phase()
{
}

std::string Phase::getModuleName()
{
	return std::string("Phase");
}

void Phase::onTick(C_GameMode* gm) {
	if(gm->player != nullptr)
		gm->player->aabb.upper.y = gm->player->aabb.lower.y;
}
void Phase::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr)
		g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
}