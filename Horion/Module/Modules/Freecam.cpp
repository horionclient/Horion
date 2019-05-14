#include "Freecam.h"



Freecam::Freecam() : IModule('V')
{
}


Freecam::~Freecam()
{
}

std::string Freecam::getModuleName()
{
	return std::string("Freecam");
}