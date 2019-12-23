#include "Criticals.h"

Criticals::Criticals() : IModule(0x0, Category::COMBAT, "Each hit is a critical hit") {
}


Criticals::~Criticals() {
}

const char* Criticals::getModuleName() {
	return ("Criticals");
}

void Criticals::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && !g_Data.getLocalPlayer()->onGround) {
		C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		movePacket->onGround = false;
	}
}