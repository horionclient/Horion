#include "Godmode.h"

Godmode::Godmode() : IModule(0, Category::MISC, "Regen health so fast you won't die") {
	registerIntSetting("regen delay", &this->regendelay, this->regendelay, 1, 20);
	registerIntSetting("regen value", &this->regenvalue, this->regenvalue, 1, 20);
}

Godmode::~Godmode() {
}

const char* Godmode::getModuleName() {
	return "Godmode";
}

void Godmode::onTick(C_GameMode* gm) {
	delay++;
	if (delay >= regendelay) {
		delay = 0;
		gm->player->causeFallDamage((float) regenvalue);
	}
}

void Godmode::onSendPacket(C_Packet* p) {
	if (p->isInstanceOf<C_MovePlayerPacket>()) {
		C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(p);
		movePacket->onGround = true;
	} /*else if (p->isInstanceOf<C_ActorFallPacket>()) {
		C_ActorFallPacket* fallPacket = reinterpret_cast<C_ActorFallPacket*>(p);
		fallPacket->fallDistance = 0.f;
	}*/
}
