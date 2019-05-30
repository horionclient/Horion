#include "RainbowSky.h"



RainbowSky::RainbowSky() : IModule('N', VISUAL)
{
}


RainbowSky::~RainbowSky()
{
}

std::string RainbowSky::getModuleName()
{
	return std::string("Rainbow Sky");
}
