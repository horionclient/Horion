#include "PacketLogger.h"



PacketLogger::PacketLogger() : IModule(0x0, Category::EXPLOITS, "X-Ray view!")
{
}


PacketLogger::~PacketLogger()
{
}

const char* PacketLogger::getModuleName()
{
	return ("PacketLogger");
}