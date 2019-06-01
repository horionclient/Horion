#include "ForceCreative.h"



ForceCreative::ForceCreative() : IModule(VK_NUMPAD2, EXPLOITS)
{
}


ForceCreative::~ForceCreative()
{
}

const char* ForceCreative::getModuleName()
{
	return ("ForceCreative");
}

void ForceCreative::onTick(C_GameMode* gm) {
	if(gm->player != nullptr && oneTime){

		C_LocalPlayer* myPlayer = reinterpret_cast<C_LocalPlayer*>(gm->player);
		oldGameMode = myPlayer->gamemode;
		myPlayer->setGameModeType(1);
		myPlayer->gamemode = 1;
		oneTime = false;
	}

}

void ForceCreative::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr) {

		g_Data.getLocalPlayer()->setGameModeType(oldGameMode);
		g_Data.getLocalPlayer()->gamemode = oldGameMode;
		oneTime = true;
	}
		
}