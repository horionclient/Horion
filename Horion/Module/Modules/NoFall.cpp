#include "NoFall.h"



NoFall::NoFall() : IModule(VK_NUMPAD3)
{
}


NoFall::~NoFall()
{
}

std::string NoFall::getModuleName()
{
	return std::string("NoFall");
}

void NoFall::onTick(C_GameMode* gm) {
	if (gm->player != nullptr) {
		gm->player->isFalling = 0;
	}
}

