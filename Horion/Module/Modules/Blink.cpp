#include "Blink.h"



Blink::Blink() : IModule('L', EXPLOITS)
{
}


Blink::~Blink()
{
	this->PacketMeme.clear();
}

std::string Blink::getModuleName()
{
	return std::string("Blink");
}

std::string Blink::getRawModuleName()
{
	return std::string("Blink_");
}
