#include "Blink.h"



Blink::Blink() : IModule('W')
{
}


Blink::~Blink()
{
}

std::string Blink::getModuleName()
{
	return std::string("Blink");
}