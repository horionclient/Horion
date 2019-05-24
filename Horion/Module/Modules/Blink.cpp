#include "Blink.h"



Blink::Blink() : IModule('W')
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


