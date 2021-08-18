#include "Xray.h"

Xray::Xray() : IModule(0, Category::VISUAL, "Allows you to see certain blocks easier") {
}

Xray::~Xray() {
}

const char* Xray::getModuleName() {
	return ("Xray");
}

void Xray::onTick(C_GameMode* gm) {
	if (smoothLightningSetting != nullptr) {
		if (!gotSmoothInfo) {
			gotSmoothInfo = true;
			wasSmooth = *smoothLightningSetting;
		}
		*smoothLightningSetting = 0;
	}
}

void Xray::onDisable() {
	if (smoothLightningSetting != nullptr && gotSmoothInfo) {
		*smoothLightningSetting = wasSmooth;
	}
	gotSmoothInfo = false;
}