#include "FullBright.h"

FullBright::FullBright() : IModule(0, Category::VISUAL, "Puts your gamma to max") {
}

FullBright::~FullBright() {
}

const char* FullBright::getModuleName() {
	return "Fullbright";
}

void FullBright::onEnable() {
	if (gammaPtr != nullptr) {
		prevGamma = *gammaPtr;
		*gammaPtr = 10;
	}
}

void FullBright::onTick(C_GameMode* gm) {
	if (gammaPtr != nullptr && *gammaPtr != 10) {
		//if player joins world get original gamma (doesn't work)
		if (prevGamma > 0.0f)
			prevGamma = *gammaPtr;
		*gammaPtr = 10;
	}
}

void FullBright::onDisable() {
	if (gammaPtr != nullptr && prevGamma > 0.0f)
		*gammaPtr = prevGamma;
	else if (gammaPtr != nullptr)
		*gammaPtr = 1.0f;
}
