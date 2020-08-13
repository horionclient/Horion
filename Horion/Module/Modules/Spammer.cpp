#include "Spammer.h"
#include "../../../Utils/Utils.h"

Spammer::Spammer() : IModule(0x0, Category::MISC, "Spams a message in a specified delay") {
	registerIntSetting("delay", &this->delay, this->delay, 1, 10);
	registerBoolSetting("bypass", &this->bypass, this->bypass);
}

Spammer::~Spammer() {
}

const char* Spammer::getModuleName() {
	return ("Spammer");
}

void Spammer::onTick(C_GameMode* gm) {
	Odelay++;
	if (Odelay > delay * 20) {
		C_TextPacket textPacket;
		textPacket.message.setText(bypass ? (message + " | " + Utils::randomString(8)) : message);
		textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
		textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
		Odelay = 0;
	}
}
