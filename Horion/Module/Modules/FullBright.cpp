#include "FullBright.h"



FullBright::FullBright() : IModule(0x0, Category::VISUAL, "Puts your gamma to max")
{
}


FullBright::~FullBright()
{
}

const char* FullBright::getModuleName()
{
	return ("Fullbright");
}

void FullBright::onTick(C_GameMode* gm)
{
	if (gammaPtr != nullptr && *gammaPtr != 10)
		*gammaPtr = 10;
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