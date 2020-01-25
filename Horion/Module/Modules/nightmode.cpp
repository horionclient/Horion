#include "nightmode.h"

NightMode::NightMode() : IModule(0, Category::VISUAL, "Darkens your surroundings") {
}

NightMode::~NightMode() {
}

const char* NightMode::getModuleName() {
	return "NightMode";
}

void NightMode::onTick(C_GameMode* gm) {
}

void NightMode::onDisable() {
}