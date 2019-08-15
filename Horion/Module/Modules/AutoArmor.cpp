#include "AutoArmor.h"

AutoArmor::AutoArmor() : IModule(0x0, PLAYER)
{
}


AutoArmor::~AutoArmor()
{
}

const char* AutoArmor::getModuleName()
{
	return ("AutoArmor");
}

void AutoArmor::onTick(C_GameMode* gm) 
{
}