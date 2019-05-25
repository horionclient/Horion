#include "GameMode.h"



GameMode::GameMode() : IModule(VK_NUMPAD2)
{
}


GameMode::~GameMode()
{
}

std::string GameMode::getModuleName()
{
	return std::string("GameMode");
}

void GameMode::onTick(C_GameMode* gm) {
	if(gm->player != nullptr && oneTime){

		C_LocalPlayer* myPlayer = reinterpret_cast<C_LocalPlayer*>(gm->player);
		oldGameMode = myPlayer->gamemode;
		myPlayer->setGameModeType(1);
		myPlayer->gamemode = 1;
		oneTime = false;
	}

}

void GameMode::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr) {

		g_Data.getLocalPlayer()->setGameModeType(oldGameMode);
		g_Data.getLocalPlayer()->gamemode = oldGameMode;
		oneTime = true;
	}
		
}