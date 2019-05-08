#include "NoKnockBack.h"

NoKnockBack::NoKnockBack() : IModule('J')
{
}

NoKnockBack::~NoKnockBack()
{
}

std::string NoKnockBack::getModuleName()
{
	return std::string("NoKnockBack");
}
