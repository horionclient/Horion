#include "RainbowSky.h"

RainbowSky::RainbowSky() : IModule(0, Category::VISUAL, "Makes your sky look rainbow!") {
}

RainbowSky::~RainbowSky() {
}

const char* RainbowSky::getModuleName() {
	return ("RainbowSky");
}
