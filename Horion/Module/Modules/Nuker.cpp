#include "Nuker.h"



Nuker::Nuker() : IModule(VK_NUMPAD5, BUILD)
{
}


Nuker::~Nuker()
{
}

std::string Nuker::getModuleName()
{
	return std::string("Nuker");
}