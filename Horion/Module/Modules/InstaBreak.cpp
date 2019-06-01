#include "InstaBreak.h"



InstaBreak::InstaBreak() : IModule(VK_NUMPAD4, BUILD)
{
}


InstaBreak::~InstaBreak()
{
}

const char* InstaBreak::getModuleName()
{
	return ("InstaBreak");
}