#include "FullBright.h"



FullBright::FullBright() : IModule(0x0, VISUAL)
{
}


FullBright::~FullBright()
{
}

const char* FullBright::getModuleName()
{
	return ("FullBright");
}

void FullBright::onEnable()
{
	if (gammaPtr != nullptr)
		*gammaPtr = 10;
}

void FullBright::onDisable()
{
	if (gammaPtr != nullptr)
		*gammaPtr = 0.5f;
}