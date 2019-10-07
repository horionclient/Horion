#include "AntiImmobile.h"


AntiImmobile::AntiImmobile() : IModule(0x0, MOVEMENT)
{
}

AntiImmobile::~AntiImmobile()
{
}

const char* AntiImmobile::getModuleName()
{
	return ("AntiImmobile");
}

void AntiImmobile::onTick(C_GameMode* gm) {

	// if (gm->player->isImmobile()) gm->player->setImmobile(false);

}