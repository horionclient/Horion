#include "PacketLogger.h"



PacketLogger::PacketLogger() : IModule(0x0, Category::EXPLOITS, "Logging Packets !") {
}


PacketLogger::~PacketLogger() {
}

const char* PacketLogger::getModuleName() {
	return ("PacketLogger");
}

void PacketLogger::onSendPacket(C_Packet* packet) {
	g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s", packet->getName()->getText());
}