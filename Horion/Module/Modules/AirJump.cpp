#include "AirJump.h"



AirJump::AirJump() : IModule('G', MOVEMENT)
{
}


AirJump::~AirJump()
{
}

std::string AirJump::getModuleName()
{
	return std::string("AirJump");
}

void AirJump::onTick(C_GameMode* gm) {
	if(gm->player != nullptr)
		gm->player->onGround = true;
}
