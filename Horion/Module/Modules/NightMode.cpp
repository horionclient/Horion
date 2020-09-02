#include "NightMode.h"

TimeChanger::TimeChanger() : IModule(0x0, Category::VISUAL, "Change the time! (Client sided)") {
	registerFloatSetting("modifier", &this->modifier, this->modifier, 0.1f, 1.f);
}

TimeChanger::~TimeChanger() {
}

const char* TimeChanger::getModuleName() {
	return "TimeChanger";
}

void TimeChanger::onEnable() {
	static auto rainbowSkyMod = moduleMgr->getModule<RainbowSky>();
	if (rainbowSkyMod->isEnabled())
		rainbowSkyMod->setEnabled(false);
}