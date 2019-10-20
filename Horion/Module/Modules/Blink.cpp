#include "Blink.h"



Blink::Blink() : IModule('L', Category::EXPLOITS, "Stops you from sending packets and then sends them in a bunch")
{
}


Blink::~Blink()
{
	this->PacketMeme.clear();
}

const char* Blink::getModuleName()
{
	return ("Blink");
}

const char* Blink::getRawModuleName()
{
	return ("Blink_");
}
