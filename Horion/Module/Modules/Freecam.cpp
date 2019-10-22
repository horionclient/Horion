#include "Freecam.h"



Freecam::Freecam() : IModule('V', Category::EXPLOITS, "Move your cam without moving the player")
{
}


Freecam::~Freecam()
{
}

const char* Freecam::getModuleName()
{
	return ("Freecam");
}

void Freecam::onTick(C_GameMode* gm) {
	if (gm->player != nullptr)
		gm->player->aabb.upper.y = gm->player->aabb.lower.y;
}

void Freecam::onEnable()
{
	if (g_Data.getLocalPlayer() != nullptr)
	{
		oldGameMode = g_Data.getLocalPlayer()->gamemode;
		oldPos = *g_Data.getLocalPlayer()->getPos();
		g_Data.getLocalPlayer()->setGameModeType(1);
		g_Data.getLocalPlayer()->gamemode = 0;
	}
}

void Freecam::onDisable()
{
	if (g_Data.getLocalPlayer() != nullptr) {
		g_Data.getLocalPlayer()->setPos(oldPos);
		g_Data.getLocalPlayer()->setGameModeType(oldGameMode);
	}
}