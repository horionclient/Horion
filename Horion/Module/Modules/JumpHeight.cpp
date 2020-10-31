#include "JumpHeight.h"

JumpHeight::JumpHeight() : IModule(0, Category::MOVEMENT, "Modify your jump height!") {
	registerFloatSetting("Jump Height", &this->jumpPower, 1, 0.f, 5.f);
}

JumpHeight::~JumpHeight() {
}

const char* JumpHeight::getModuleName() {
	return ("JumpHeight");
}
