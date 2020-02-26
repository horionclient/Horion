#include "NightMode.h"

NightMode::NightMode() : IModule(0x0, Category::VISUAL, "") {
	registerFloatSetting("modifier", &this->modifier, this->modifier, 0.1f, 1.f);
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