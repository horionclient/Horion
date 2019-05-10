#include "RainbowSky.h"



RainbowSky::RainbowSky() : IModule('N')
{
}


RainbowSky::~RainbowSky()
{
}

std::string RainbowSky::getModuleName()
{
	return std::string("Rainbow Sky");
}
