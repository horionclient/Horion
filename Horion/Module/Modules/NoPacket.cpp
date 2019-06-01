#include "NoPacket.h"



NoPacket::NoPacket() : IModule(VK_NUMPAD7, EXPLOITS)
{
}


NoPacket::~NoPacket()
{

}

const char* NoPacket::getModuleName()
{
	return ("NoPacket");
}


