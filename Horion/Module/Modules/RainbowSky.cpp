#include "RainbowSky.h"



RainbowSky::RainbowSky() : IModule('N', Category::VISUAL, "Makes your sky look rainbow!")
{
}


RainbowSky::~RainbowSky()
{
}

const char* RainbowSky::getModuleName()
{
	return ("RainbowSky");
}
