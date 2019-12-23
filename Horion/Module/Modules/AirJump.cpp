#include "AirJump.h"


AirJump::AirJump() : IModule('G', Category::MOVEMENT, "Jump even you're not on the ground") {
}


AirJump::~AirJump() {
}

const char* AirJump::getModuleName() {
	return ("AirJump");
}

void AirJump::onTick(C_GameMode* gm) {

	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	if (gm->player == nullptr || input == nullptr)
		return;
	
	if (GameData::isKeyDown(*input->spaceBarKey) && hasJumped == 0) {
		gm->player->onGround = true;
		hasJumped = 1;
	} 
	else if (!GameData::isKeyDown(*input->spaceBarKey)) {
		hasJumped = 0;
	}
}
