#include "NoPacket.h"



NoPacket::NoPacket() : IModule(VK_NUMPAD7, EXPLOITS, "Prevents you from sending InventoryTransaction packets")
{
}


NoPacket::~NoPacket()
{

}

const char* NoPacket::getModuleName()
{
	return ("NoPacket");
}


