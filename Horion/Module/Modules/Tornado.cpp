#include "Tornado.h"

Tornado::Tornado() : IModule(0, Category::COMBAT, "Makes you small and damage entities you run into when you jump.") {
}
Tornado::~Tornado() {
}

const char* Tornado::getModuleName() {
	return "Tornado";
}

void Tornado::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();

	if (player->isJumping()) {
		player->setDamageNearbyMobs(true);
	} else if (!player->isJumping()) {
		player->setDamageNearbyMobs(false);
	}
}