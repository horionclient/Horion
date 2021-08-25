#include "Blink.h"

Blink::Blink() : IModule(0, Category::MISC, "Stops you from sending packets and then sends them in a bunch") {
}

Blink::~Blink() {
	getMovePlayerPacketHolder()->clear();
	getPlayerAuthInputPacketHolder()->clear();
}

const char* Blink::getModuleName() {
	return ("Blink");
}

void Blink::onTick(C_GameMode* gm) {
	gm->player->fallDistance = 0.f;
}
