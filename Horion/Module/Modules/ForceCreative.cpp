#include "ForceCreative.h"



ForceCreative::ForceCreative() : IModule(VK_NUMPAD2)
{
}


ForceCreative::~ForceCreative()
{
}

std::string ForceCreative::getModuleName()
{
	return std::string("ForceCreative");
}

void ForceCreative::onTick(C_GameMode* gm) {
	if(gm->player != nullptr && oneTime){

		C_LocalPlayer* myPlayer = reinterpret_cast<C_LocalPlayer*>(gm->player);
		oldGameMode = myPlayer->gamemode;
		myPlayer->setGameModeType(1);
		oneTime = false;
	}

}

void ForceCreative::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr) {

		g_Data.getLocalPlayer()->setGameModeType(oldGameMode);
		oneTime = true;
	}
		
}