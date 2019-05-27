#include "Nuker.h"



Nuker::Nuker() : IModule('Y')
{
}


Nuker::~Nuker()
{
}

std::string Nuker::getModuleName()
{
	return std::string("Nuker");
}