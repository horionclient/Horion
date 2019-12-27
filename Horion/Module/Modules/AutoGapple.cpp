#include "AutoGapple.h"


AutoGapple::AutoGapple() : IModule(0x0, Category::MOVEMENT, "Auto eat gapples if you're low health")
{
	registerIntSetting("health", &this->health, 10, 1, 20);
}

AutoGapple::~AutoGapple()
{
}

const char* AutoGapple::getModuleName()
{
	return ("AutoGapple");
}

void AutoGapple::onTick(C_GameMode* gm) {

	if (gm->player != nullptr) return;
	
}