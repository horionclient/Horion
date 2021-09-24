#include "AirJump.h"

AirJump::AirJump() : IModule('G', Category::MOVEMENT, "Jump even you're not on the ground") {
	registerBoolSetting("Selfhurt", &this->selfhurt, this->selfhurt);
	registerBoolSetting("Legacy", &legacyMode, legacyMode);
}

AirJump::~AirJump() {
}

void AirJump::onEnable() {
	if (selfhurt) {
		g_Data.getLocalPlayer()->swing();
		g_Data.getCGameMode()->attack(g_Data.getLocalPlayer());
	}
}

const char* AirJump::getModuleName() {
	return ("AirJump");
}

void AirJump::onTick(C_GameMode* gm) {
	if (legacyMode) {
		gm->player->onGround = true;
		return;
	}
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	if (input == nullptr)
		return;

	if (GameData::isKeyDown(*input->spaceBarKey) && hasJumped == 0) {
		gm->player->onGround = true;
		hasJumped = 1;
	} else if (!GameData::isKeyDown(*input->spaceBarKey)) {
		hasJumped = 0;
	}
}
