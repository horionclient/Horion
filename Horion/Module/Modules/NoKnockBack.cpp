#include "NoKnockBack.h"

NoKnockBack::NoKnockBack() : IModule('J', MOVEMENT)
{
}

NoKnockBack::~NoKnockBack()
{
}

std::string NoKnockBack::getModuleName()
{
	return std::string("NoKnockBack");
}
