#include "Velocity.h"

Velocity::Velocity() : IModule('J', Category::COMBAT, "Take More Or Less Knockback") {
	this->registerFloatSetting("Linear Modifier", &this->xModifier, this->xModifier, 0.f, 4.f);
	this->registerFloatSetting("Height Modifier", &this->yModifier, this->yModifier, 0.f, 4.f);
}

Velocity::~Velocity() {
}

const char* Velocity::getModuleName() {
	return ("Velocity");
}
