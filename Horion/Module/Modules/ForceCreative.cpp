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
	if(gm->player != nullptr){
		C_LocalPlayer* myPlayer = reinterpret_cast<C_LocalPlayer*>(gm->player);
		if (myPlayer->gamemode != 1) {
			oldGameMode = myPlayer->gamemode;
			myPlayer->setGameModeType(1);
			myPlayer->gamemode = 1;
		}
	}
}

void ForceCreative::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr) {
		g_Data.getLocalPlayer()->setGameModeType(oldGameMode);
		g_Data.getLocalPlayer()->gamemode = oldGameMode;
	}
}