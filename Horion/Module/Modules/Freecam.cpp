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
	gm->player->aabb.upper.y = gm->player->aabb.lower.y;
	gm->player->canFly = true;
}

void Freecam::onEnable()
{
	if (g_Data.getLocalPlayer() != nullptr)
	{
		oldPos = *g_Data.getLocalPlayer()->getPos();
	}
}

void Freecam::onDisable()
{
	if (g_Data.getLocalPlayer() != nullptr) {
		g_Data.getLocalPlayer()->setPos(oldPos);
		g_Data.getLocalPlayer()->canFly = false;
	}
}