#include "Freecam.h"



Freecam::Freecam() : IModule('V', EXPLOITS)
{
}


Freecam::~Freecam()
{
}

std::string Freecam::getModuleName()
{
	return std::string("Freecam");
}