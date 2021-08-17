#include "FullBright.h"

FullBright::FullBright() : IModule(0, Category::VISUAL, "increases your gamma") {
	this->registerFloatSetting("Gamma", &this->gamma, this->gamma, 0.25f, 10.f);
}

FullBright::~FullBright() {
}

const char* FullBright::getModuleName() {
	return "Fullbright";
}

void FullBright::onTick(C_GameMode* gm) {
	if (gammaPtr != nullptr)
		*gammaPtr = gamma;
}

void FullBright::onEnable() {
	if (gammaPtr != nullptr)
		*gammaPtr = gamma;
}

void FullBright::onDisable() {
	if (gammaPtr != nullptr)
		*gammaPtr = 0.5f;
}
