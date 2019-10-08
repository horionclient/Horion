#include "AutoGapple.h"


AutoGapple::AutoGapple() : IModule(0x0, MOVEMENT)
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