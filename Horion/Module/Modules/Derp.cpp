#include "Derp.h"



Derp::Derp() : IModule(0x0, Category::EXPLOITS, "lol you stupid") {
	this->registerBoolSetting("ihaveastroke", &this->epicStroke, this->epicStroke);
	this->registerBoolSetting("packet mode", &this->packetMode, this->packetMode);
}


Derp::~Derp() {

}

const char* Derp::getModuleName() {
	return "Derp";
}

void Derp::onTick(C_GameMode* gm) {
	if (gm->player == nullptr) return;

	if (packetMode) {
		C_MovePlayerPacket* p = new C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
		if (epicStroke) {
			p->pitch = (float)(rand() % 360);
			p->yaw = (float)(rand() % 360);
		}
		else {
			p->pitch = (float)(counter % 360);
			p->yaw = (float)(counter % 360);
		}
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(p);
	}
	else {
		if (epicStroke) {
			gm->player->pitch = (float)(rand() % 360);
			gm->player->bodyYaw = (float)(rand() % 360);
		}
		else {
			gm->player->pitch = (float)(counter % 360);
			gm->player->bodyYaw = (float)(counter % 360);
		}
	}
	

	if (counter < 360) counter++;
	else counter = 0;
}