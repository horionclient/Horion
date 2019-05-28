#include "NoPacket.h"



NoPacket::NoPacket() : IModule(VK_NUMPAD7)
{
}


NoPacket::~NoPacket()
{
	this->PacketMeme.clear();
}

std::string NoPacket::getModuleName()
{
	return std::string("NoPacket");
}


