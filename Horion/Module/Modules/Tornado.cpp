#include "Tornado1.h"

Tornado::Tornado() : IModule(0, Category::COMBAT, "Makes you small and damage entities you run into when you jump.") {
	registerBoolSetting("Only While Jumping", &this->onlyjump, this->onlyjump);
}
Tornado::~Tornado() {
}

const char* Tornado::getModuleName() {
	return "Tornado";
}

void Tornado::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	bool onlyJump = this->onlyjump;

	if (onlyJump) {
		if (player->isJumping()) {
			player->setDamageNearbyMobs(true);
		} else if (player->onGround) {
			player->setDamageNearbyMobs(false);
		}
	} else {
		player->setDamageNearbyMobs(true);
	}
}
