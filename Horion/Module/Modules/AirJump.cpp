#include "AirJump.h"

AirJump::AirJump() : IModule('G', Category::MOVEMENT, "Jump even you're not on the ground") {
	registerBoolSetting("Legacy", &legacyMode, legacyMode);
}

AirJump::~AirJump() {
}

const char* AirJump::getModuleName() {
	return ("AirJump");
}

void AirJump::onTick(C_Player* player) {
	if (legacyMode) {
		player->onGround = true;
		return;
	}
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	if (input == nullptr)
		return;

	if (GameData::isKeyDown(*input->spaceBarKey) && hasJumped == 0) {
		player->onGround = true;
		hasJumped = 1;
	} else if (!GameData::isKeyDown(*input->spaceBarKey)) {
		hasJumped = 0;
	}
}
