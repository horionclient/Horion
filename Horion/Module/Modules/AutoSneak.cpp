#include "AutoSneak.h"

AutoSneak::AutoSneak() : IModule(0, Category::MOVEMENT, "Automatically sneak without holding the key") {
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
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	if (!doSilent) {
		g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
		return;
	}

	C_PlayerActionPacket p;
	p.action = 12;  //stop crouch packet
	p.entityRuntimeId = g_Data.getLocalPlayer()->entityRuntimeId;
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
}

void AutoSneak::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return; //fixed crash
	
	if (doSilent) {
		C_PlayerActionPacket p;
		p.action = 11;  //start crouch packet
		p.entityRuntimeId = g_Data.getLocalPlayer()->entityRuntimeId;
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
	}
}
