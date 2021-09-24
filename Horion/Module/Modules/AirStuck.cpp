#include "AirStuck.h"

AirStuck::AirStuck() : IModule(0, Category::MOVEMENT, "Become stuck.") {
	registerBoolSetting("Selfhurt", &this->selfhurt, this->selfhurt);
}

AirStuck::~AirStuck() {
}

void AirStuck::onEnable() {
	if (selfhurt) {
		g_Data.getLocalPlayer()->swing();
		g_Data.getCGameMode()->attack(g_Data.getLocalPlayer());
	}
}

const char* AirStuck::getModuleName() {
	return ("AirStuck");
}

void AirStuck::onTick(C_GameMode* gm) {
	gm->player->velocity = vec3_t(0, 0, 0);
}