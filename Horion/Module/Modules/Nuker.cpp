#include "Nuker.h"



Nuker::Nuker() : IModule(VK_NUMPAD5)
{
}


Nuker::~Nuker()
{
}

std::string Nuker::getModuleName()
{
	return std::string("Nuker");
}