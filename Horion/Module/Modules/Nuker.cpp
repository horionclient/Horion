#include "Nuker.h"



Nuker::Nuker() : IModule(VK_NUMPAD5, BUILD)
{
}


Nuker::~Nuker()
{
}

const char* Nuker::getModuleName()
{
	return ("Nuker");
}