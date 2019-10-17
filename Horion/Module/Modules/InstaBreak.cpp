#include "InstaBreak.h"



InstaBreak::InstaBreak() : IModule(VK_NUMPAD4, BUILD, "Break any block instantly")
{
}


InstaBreak::~InstaBreak()
{
}

const char* InstaBreak::getModuleName()
{
	return ("InstaBreak");
}