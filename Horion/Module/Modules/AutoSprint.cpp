#include "AutoSprint.h"

AutoSprint::AutoSprint() : IModule(VK_SHIFT, MOVEMENT)
{
}


AutoSprint::~AutoSprint()
{
}

const char* AutoSprint::getModuleName()
{
	return ("AutoSprint");
}

void AutoSprint::onTick(C_GameMode* gm) {
	if (gm->player != nullptr && !gm->player->isSprinting() && GameData::canUseMoveKeys() && gm->player->velocity.magnitudexz() > 0.01f)
		gm->player->setSprinting(true);
}