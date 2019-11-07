#include "Derp.h"



Derp::Derp() : IModule(0x0, Category::EXPLOITS, "lol you stupid")
{
}


Derp::~Derp()
{

}

const char* Derp::getModuleName()
{
	return "Derp";
}

void Derp::onTick(C_GameMode* gm)
{
	if (gm->player == nullptr) return;
	gm->player->pitch = (float) c;
	gm->player->bodyYaw = (float) c;
	if (c < 360) c++;
	else c = 0;
}