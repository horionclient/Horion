#include "NoPacket.h"



NoPacket::NoPacket() : IModule(VK_NUMPAD7, EXPLOITS)
{
}


NoPacket::~NoPacket()
{

}

std::string NoPacket::getModuleName()
{
	return std::string("NoPacket");
}


