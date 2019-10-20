#include "AutoSprint.h"

AutoSprint::AutoSprint() : IModule(0x0, Category::MOVEMENT, "Automatically sprint without holding the key")
{
}


AutoSprint::~AutoSprint()
{
}

const char* AutoSprint::getModuleName()
{
	return ("AutoSprint");
}

const char* AutoSprint::getRawModuleName()
{
	return ("AutoSprint_");
}

void AutoSprint::onTick(C_GameMode* gm) {
	if (gm->player != nullptr && !gm->player->isSprinting() && GameData::canUseMoveKeys() && gm->player->velocity.magnitudexz() > 0.01f) {
		gm->player->setSprinting(true);
	}
		
}