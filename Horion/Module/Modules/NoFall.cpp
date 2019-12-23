#include "NoFall.h"



NoFall::NoFall() : IModule(VK_NUMPAD3, Category::MOVEMENT, "Prevents you from taking falldamage") {
}


NoFall::~NoFall() {
}

const char* NoFall::getModuleName() {
	return ("NoFall");
}

void NoFall::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		movePacket->onGround = true;
	}
	else if (packet->isInstanceOf<C_ActorFallPacket>()) {
		C_ActorFallPacket* fallPacket = reinterpret_cast<C_ActorFallPacket*>(packet);
		fallPacket->fallDistance = 0.f;
	}
}