#include "AutoWalk.h"

AutoWalk::AutoWalk() : IModule(0, Category::MOVEMENT, "Automatically walk for you") {
	mode = (*new SettingEnum(this)).addEntry(EnumEntry("Forwards", 1)).addEntry(EnumEntry("Left", 2)).addEntry(EnumEntry("Right", 3)).addEntry(EnumEntry("Backwards", 4));
	this->registerEnumSetting("Direction", &mode, 0);
	this->registerBoolSetting("Sprint", &this->sprint, this->sprint);
	this->registerBoolSetting("Jump", &this->jump, this->jump);
}

AutoWalk::~AutoWalk() {}

const char* AutoWalk::getModuleName() {
	return ("AutoWalk");
}

void AutoWalk::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();

	if (mode.selected == 0) g_Data.getClientInstance()->getMoveTurnInput()->forward = true;
	if (mode.selected == 1) g_Data.getClientInstance()->getMoveTurnInput()->left = true;
	if (mode.selected == 2) g_Data.getClientInstance()->getMoveTurnInput()->right = true;
	if (mode.selected == 3) g_Data.getClientInstance()->getMoveTurnInput()->backward = true;

	if (sprint) gm->player->setSprinting(true);
		else gm->player->setSprinting(false);

	if (player->onGround && jump) gm->player->jumpFromGround();
}

void AutoWalk::onDisable() {
	g_Data.getClientInstance()->getMoveTurnInput()->forward = false;
	g_Data.getClientInstance()->getMoveTurnInput()->left = false;
	g_Data.getClientInstance()->getMoveTurnInput()->right = false;
	g_Data.getClientInstance()->getMoveTurnInput()->backward = false;
}