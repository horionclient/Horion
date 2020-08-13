#include "Fly.h"

Fly::Fly() : IModule(0x0, Category::MOVEMENT, "Enables fly like in creative mode") {
}

Fly::~Fly() {
}

const char* Fly::getModuleName() {
	return ("Fly");
}

void Fly::onTick(C_GameMode* gm) {
	gm->player->canFly = true;

}

void Fly::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr)
		if (g_Data.getLocalPlayer()->gamemode != 1)
			g_Data.getLocalPlayer()->canFly = false;
}
