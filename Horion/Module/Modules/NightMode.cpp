#include "NightMode.h"

NightMode::NightMode() : IModule(0x0, Category::VISUAL, "Darkens the environment") {
}

NightMode::~NightMode() {
}

const char* NightMode::getModuleName() {
	return "NightMode";
}

void NightMode::onEnable() {
	IModule* mod = moduleMgr->getModule<RainbowSky>();
	if (mod != nullptr && mod->isEnabled())
		mod->setEnabled(false);
}