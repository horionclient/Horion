#include "Blink.h"



Blink::Blink() : IModule('L')
{
}


Blink::~Blink()
{
	this->PacketMeme.clear();
}

std::string Blink::getModuleName()
{
	return std::string("BlinK");
}


