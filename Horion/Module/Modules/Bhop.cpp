#include "Bhop.h"



Bhop::Bhop() : IModule(0x0, Category::MOVEMENT, "Hop around like a bunny!") {
	registerFloatSetting("Speed", &this->speed, this->speed, 0.1f, 0.8f);
}


Bhop::~Bhop() {
}

const char* Bhop::getModuleName() {
	return ("BunnyHop");
}

void Bhop::onTick(C_GameMode* gm) {

	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	if (gm->player->isInLava() == 1 || gm->player->isInWater() == 1) return;

	if (gm->player->isSneaking()) return;

	if (GameData::canUseMoveKeys() || g_Data.getLocalPlayer()->canOpenContainerScreen() != 1)
	{ }
	else
		return;

	if (input == nullptr) return;

	float yaw = gm->player->yaw;

	if (gm->player->onGround && (GameData::isKeyDown(*input->forwardKey) || GameData::isKeyDown(*input->backKey) || GameData::isKeyDown(*input->rightKey) || GameData::isKeyDown(*input->leftKey))) gm->player->jumpFromGround();

	if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->backKey))
		return;
	else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
		yaw += 45.f;
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
		yaw -= 45.f;
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
		yaw += 135.f;
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
		yaw -= 135.f;
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->forwardKey)) {
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->backKey)) {
		yaw += 180.f;
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
		yaw += 90.f;
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
		yaw -= 90.f;
		keyPressed = true;
	}
	if (yaw >= 180)
		yaw -= 360.f;

	float calcYaw = (yaw + 90) * (PI / 180);
	vec3_t moveVec;
	moveVec.x = cos(calcYaw) * speed;
	moveVec.y = gm->player->velocity.y;
	moveVec.z = sin(calcYaw) * speed;
	if (keyPressed) {
		gm->player->lerpMotion(moveVec);
		keyPressed = false;
	}

}