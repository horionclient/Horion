#include "Bhop.h"



Bhop::Bhop() : IModule(0x0, MOVEMENT)
{
	registerFloatSetting("Speed", &this->speed, 0.4f, 0.1f, 0.8f);
}


Bhop::~Bhop()
{
}

const char* Bhop::getModuleName()
{
	return ("Bhop");
}

void Bhop::onTick(C_GameMode* gm) {

	C_GameSettingsInput* input = g_Data.getGameSettingsInput();

	if (gm->player->isInLava() == 1 || gm->player->isInWater() == 1) return;

	if(!GameData::canUseMoveKeys()) return;

	if (input == nullptr) return;
	
	if (inputHandler == nullptr) return;

	float yaw = gm->player->yaw;

	inputHandler->ClearMovementState();

	if (gm->player->onGround && (GameData::isKeyDown(*input->forwardKey) || GameData::isKeyDown(*input->backKey) || GameData::isKeyDown(*input->rightKey) || GameData::isKeyDown(*input->leftKey))) gm->player->jumpFromGround();

	if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->backKey))
		return;
	else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey))
	{
		yaw += 45.f;
		inputHandler->isMovingForward = true;
		inputHandler->isMovingRight = true;
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey))
	{
		yaw -= 45.f;
		inputHandler->isMovingForward = true;
		inputHandler->isMovingLeft = true;
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey))
	{
		yaw += 135.f;
		inputHandler->isMovingBackward = true;
		inputHandler->isMovingRight = true;
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey))
	{
		yaw -= 135.f;
		inputHandler->isMovingBackward = true;
		inputHandler->isMovingLeft = true;
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->forwardKey))
	{
		inputHandler->isMovingForward = true;
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->backKey))
	{
		yaw += 180.f;
		inputHandler->isMovingBackward = true;
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey))
	{
		yaw += 90.f;
		inputHandler->isMovingRight = true;
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey))
	{
		yaw -= 90.f;
		inputHandler->isMovingLeft = true;
		keyPressed = true;
	}
	if (yaw >= 180)
		yaw -= 360.f;

	float calcYaw = (yaw + 90) * (PI / 180);
	float calcPitch = (gm->player->pitch) * -(PI / 180);
	vec3_t moveVec;
	moveVec.x = cos(calcYaw) * cos(calcPitch) * speed;
	moveVec.y = gm->player->velocity.y;
	moveVec.z = sin(calcYaw) * cos(calcPitch) * speed;
	if (keyPressed)
	{
		gm->player->lerpMotion(moveVec);
		keyPressed = false;
	}

}