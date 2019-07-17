#include "AirJump.h"


AirJump::AirJump() : IModule('G', MOVEMENT)
{
}


AirJump::~AirJump()
{
}

const char* AirJump::getModuleName()
{
	return ("AirJump");
}

void AirJump::onTick(C_GameMode* gm) {
	static IModule* InvMovMod = moduleMgr->getModule<InventoryMove>();
	if(gm->player != nullptr && InvMovMod!= nullptr && !InvMovMod->isEnabled())
		gm->player->onGround = true;
}
