#include "AutoWalk.h"

AutoWalk::AutoWalk() : IModule(0, Category::MOVEMENT, "Automatically walk without pressing the key") {
	registerFloatSetting("speed", &this->speed, this->speed, 0.1f, 1.f);
}

AutoWalk::~AutoWalk() {
}

const char* AutoWalk::getModuleName() {
	return "AutoWalk";
}

void AutoWalk::onTick(C_GameMode* gm) {
	float calcYaw = (gm->player->yaw + 90) * (PI / 180);
	gm->player->velocity.x = cos(calcYaw) * speed;
	gm->player->velocity.z = sin(calcYaw) * speed;
}
