#include "PacketLogger.h"



PacketLogger::PacketLogger() : IModule(0x0, Category::EXPLOITS, "Logging Packets !")
{
}


PacketLogger::~PacketLogger()
{
}

const char* PacketLogger::getModuleName()
{
	return ("PacketLogger");
}