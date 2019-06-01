#include "RainbowSky.h"



RainbowSky::RainbowSky() : IModule('N', VISUAL)
{
}


RainbowSky::~RainbowSky()
{
}

const char* RainbowSky::getModuleName()
{
	return ("RainbowSky");
}
