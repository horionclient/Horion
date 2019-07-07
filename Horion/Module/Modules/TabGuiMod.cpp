#include "TabGuiMod.h"



TabGuiMod::TabGuiMod() : IModule(0x0, VISUAL)
{
}


TabGuiMod::~TabGuiMod()
{
}

const char* TabGuiMod::getModuleName()
{
	return ("TabGui");
}

