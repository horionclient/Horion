#include "Killaura.h"



Killaura::Killaura() : IModule('P') // <-- keybind
{
}


Killaura::~Killaura()
{
}

std::string Killaura::getModuleName()
{
	return std::string("Killaura");
}

