#include "Blink.h"



Blink::Blink() : IModule('W')
{
}


Blink::~Blink()
{
	PacketMeme.clear();
}

std::string Blink::getModuleName()
{
	return std::string("Blink");
}

void Blink::AddPackets(C_MovePlayerPacket* a)
{
	this->PacketMeme.push_back(C_MovePlayerPacket(*a));
}

void Blink::sendPackets()
{
	if (!this->isEnabled() && this->PacketMeme.size() >= 1)
	{

	}
}