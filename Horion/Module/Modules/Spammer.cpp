#include "Spammer.h"
#include <random>
#include <string>

Spammer::Spammer() : IModule(0x0, Category::EXPLOITS, "Spams a message in a specified delay")
{
	registerIntSetting("delay", &this->delay, this->delay, 1, 10);
	registerBoolSetting("bypass", &this->bypass, this->bypass);
}


Spammer::~Spammer()
{
}

const char* Spammer::getModuleName()
{
	return ("Spammer");
}

void Spammer::onTick(C_GameMode* gm)
{
	Odelay++;
	if (Odelay > delay * 20) {
		C_TextPacket* textPacket = new C_TextPacket();
		textPacket->message.setText(bypass ? (message + " | " + random()) : message);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(textPacket);
		delete textPacket;
		Odelay = 0;
	}
}

std::string Spammer::random() {
	std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	std::random_device rd;
	std::mt19937 generator(rd());
	std::shuffle(str.begin(), str.end(), generator);
	return str.substr(0, 8);
}