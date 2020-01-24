#include "nightmode.h"

Nightmode::Nightmode() : IModule(0, Category::VISUAL, "Darkens your surroundings") {
	this->registerBoolSetting("Full Dark", &this->fdark, true);
	this->registerBoolSetting("Purple Dark", &this->mdark, true);
}

Nightmode::~Nightmode() {

}

const char* Nightmode::getModuleName() {
	return "Nightmode";
}

void Nightmode::onTick(C_GameMode* gm) {
}

void Nightmode::onDisable() {
}

