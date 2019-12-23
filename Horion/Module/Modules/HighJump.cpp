#include "HighJump.h"



HighJump::HighJump() : IModule(0x0, Category::MOVEMENT, "Jump higher than ever before") {
	registerFloatSetting("Jump Height", &this->jumpPower, 1, 0.5f, 5.f);
}


HighJump::~HighJump() {
}

const char* HighJump::getModuleName() {
	return ("HighJump");
}


