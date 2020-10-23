#include "AutoSneak.h"

AutoSneak::AutoSneak() : IModule(0x0, Category::MOVEMENT, "Automatically sneak without holding the key") {
	this->registerBoolSetting("silent", &this->doSilent, doSilent);
}

AutoSneak::~AutoSneak() {
}

const char* AutoSneak::getModuleName() {
	return ("AutoSneak");
}

void AutoSneak::onTick(C_GameMode* gm) {

	if (!doSilent)
		g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = true;
}

void AutoSneak::onDisable() {
	C_PlayerActionPacket p;
	p.action = 12;  //stop crouch packet


	

	if (g_Data.getLocalPlayer() != nullptr) {

		if (doSilent)
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);

		if (!doSilent)
			g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
	}
}


void AutoSneak::onSendPacket(C_Packet* packet) {
	//wait i dont actually need this bit lol

}

void AutoSneak::onEnable() {
	C_PlayerActionPacket p;
	p.action = 11;  //start crouch packet

	if (doSilent)
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);

}
