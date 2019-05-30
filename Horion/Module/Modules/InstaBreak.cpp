#include "InstaBreak.h"



InstaBreak::InstaBreak() : IModule(VK_NUMPAD4, BUILD)
{
}


InstaBreak::~InstaBreak()
{
}

std::string InstaBreak::getModuleName()
{
	return std::string("InstaBreak");
}