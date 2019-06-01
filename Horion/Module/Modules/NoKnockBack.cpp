#include "NoKnockBack.h"

NoKnockBack::NoKnockBack() : IModule('J', MOVEMENT)
{
}

NoKnockBack::~NoKnockBack()
{
}

const char* NoKnockBack::getModuleName()
{
	return ("NoKnockBack");
}
