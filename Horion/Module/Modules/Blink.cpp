#include "Blink.h"



Blink::Blink() : IModule('L', EXPLOITS)
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
