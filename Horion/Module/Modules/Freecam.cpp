#include "Freecam.h"



Freecam::Freecam() : IModule('V', EXPLOITS)
{
}


Freecam::~Freecam()
{
}

const char* Freecam::getModuleName()
{
	return ("Freecam");
}