#include "AutoWalk.h"

AutoWalk::AutoWalk() : IModule(0, Category::MOVEMENT, "Automatically walk for you") {
	this->registerBoolSetting("Sprint", &this->sprint, this->sprint);
	this->registerBoolSetting("Jump", &this->jump, this->jump);
}

AutoWalk::~AutoWalk() {}

const char* AutoWalk::getModuleName() {
	return ("AutoWalk");
}

void AutoWalk::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();

	g_Data.getClientInstance()->getMoveTurnInput()->forward = true;
	
	if (sprint) gm->player->setSprinting(true);
	if (!sprint) gm->player->setSprinting(false);

	if (player->onGround && jump) gm->player->jumpFromGround();
}

void AutoWalk::onDisable() {
	g_Data.getClientInstance()->getMoveTurnInput()->forward = false;
}