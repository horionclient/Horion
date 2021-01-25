#include "HitboxSize1.h"

HitboxSize::HitboxSize() : IModule(0, Category::PLAYER, "Modifies your hitbox size so you can fit in very tiny holess.") {
	registerFloatSetting("Width", &this->width, this->height, 0, 10);
	registerFloatSetting("Height", &this->height, this->height, 0, 10);
}

HitboxSize::~HitboxSize() {
}

const char* HitboxSize::getModuleName() {
	return "Player Hitbox";
}

void HitboxSize::onTick(C_GameMode* gm) {
	if (this->active) {
		g_Data.getLocalPlayer()->setSize(this->width, this->height);
	}
}

void HitboxSize::onEnable() {
	this->ogheight = g_Data.getLocalPlayer()->height;
	this->ogwidth = g_Data.getLocalPlayer()->width;
	this->active = true;
}

void HitboxSize::onDisable() {
	this->active = false;
	g_Data.getLocalPlayer()->setSize(this->ogwidth, this->ogheight);
}