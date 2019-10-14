#include "NoFall.h"



NoFall::NoFall() : IModule(VK_NUMPAD3, MOVEMENT)
{
}


NoFall::~NoFall()
{
}

const char* NoFall::getModuleName()
{
	return ("NoFall");
}

void NoFall::onTick(C_GameMode* gm) {
    if (gm->player != nullptr && gm->player->fallDistance > 0) {
        gm->player->fallDistance = 0.5;
    }
}

