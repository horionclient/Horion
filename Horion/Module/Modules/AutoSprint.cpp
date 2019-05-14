#include "AutoSprint.h"

AutoSprint::AutoSprint() : IModule(VK_SHIFT)
{
}


AutoSprint::~AutoSprint()
{
}

std::string AutoSprint::getModuleName()
{
	return std::string("AutoSprint");
}

void AutoSprint::onTick(C_GameMode* gm) {
	if (gm->player != nullptr)
		gm->player->setSprinting(true);
}