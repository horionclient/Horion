#include "HitboxSize1.h"

HitboxSize::HitboxSize() : IModule(0, Category::PLAYER, "Modifies your hitbox size so you can fit in very tiny holes.") {
	registerFloatSetting("PlayerSize", &this->playersize, this->playersize, 0, 10);
}

HitboxSize::~HitboxSize() {
}

const char* HitboxSize::getModuleName() {
	return "Hitbox Size";
}

void HitboxSize::onTick(C_GameMode* gm) {
	if (this->active) {
		g_Data.getLocalPlayer()->setSize(this->playersize / 2, this->playersize);
	}
}

void HitboxSize::onEnable() {
	this->ogsize = g_Data.getLocalPlayer()->height;
	this->active = true;
}

void HitboxSize::onDisable() {
	this->active = false;
	g_Data.getLocalPlayer()->setSize(this->ogsize / 2, this->ogsize);
}
