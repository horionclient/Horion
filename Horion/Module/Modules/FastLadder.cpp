#include "FastLadder.h"

FastLadder::FastLadder() : IModule(0, Category::MOVEMENT, "Climb up ladders faster") {
	registerFloatSetting("Speed", &speed, speed, 0.1f, 1.0f);
}

FastLadder::~FastLadder() {
}

const char* FastLadder::getModuleName() {
	return ("FastLadder");
}
