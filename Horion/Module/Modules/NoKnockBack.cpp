#include "NoKnockBack.h"

NoKnockBack::NoKnockBack() : IModule('J', Category::MOVEMENT, "Don't ever take knockback again") {
	this->registerFloatSetting("Linear Modifier", &this->xModifier, this->xModifier, 0.f, 1.f);
	this->registerFloatSetting("Height Modifier", &this->yModifier, this->yModifier, 0.f, 1.f);
}

NoKnockBack::~NoKnockBack() {
}

const char* NoKnockBack::getModuleName() {
	return ("NoKnockBack");
}
