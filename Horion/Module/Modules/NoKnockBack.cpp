#include "NoKnockBack.h"

NoKnockBack::NoKnockBack() : IModule('J', MOVEMENT)
{
	this->registerFloatSetting("X & Z Modifier", &this->xModifier, this->xModifier, 0.f, 1.f);
	this->registerFloatSetting("Y Modifier", &this->yModifier, this->yModifier, 0.f, 1.f);
}

NoKnockBack::~NoKnockBack()
{
}

const char* NoKnockBack::getModuleName()
{
	return ("NoKnockBack");
}
