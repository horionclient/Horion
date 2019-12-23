#include "Freecam.h"



Freecam::Freecam() : IModule('V', Category::EXPLOITS, "Move your cam without moving the player") {
}


Freecam::~Freecam() {
}

const char* Freecam::getModuleName() {
	return ("Freecam");
}

void Freecam::onTick(C_GameMode* gm) {
	gm->player->aabb.upper.y = gm->player->aabb.lower.y;
	gm->player->canFly = true;
	gm->player->fallDistance = 0.f;
}

void Freecam::onEnable() {
	if (g_Data.getLocalPlayer() != nullptr) {
		oldPos = *g_Data.getLocalPlayer()->getPos();
	}
}

void Freecam::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr) {
		g_Data.getLocalPlayer()->setPos(oldPos);
		if(g_Data.getLocalPlayer()->gamemode != 1)
			g_Data.getLocalPlayer()->canFly = false;
	}
}