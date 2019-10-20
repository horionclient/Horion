#include "InventoryMove.h"



InventoryMove::InventoryMove() : IModule(0x0, Category::MOVEMENT, "Move even though you have your inventory open")
{
}


InventoryMove::~InventoryMove()
{
}

const char* InventoryMove::getModuleName()
{
	return ("InventoryMove");
}

void InventoryMove::onTick(C_GameMode* gm)
{
	if (g_Data.getLocalPlayer()->isInventoryClosed() == 1)
		return;
	C_GameSettingsInput* input = g_Data.getGameSettingsInput();

	if (input == nullptr)
		return;
	else if (inputHandler == nullptr)
		return;

	float speed = 0.325f;
	float yaw = gm->player->yaw;

	inputHandler->ClearMovementState();

	if (GameData::isKeyDown(*input->spaceBarKey) && gm->player->onGround)
		gm->player->jumpFromGround();

	if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->backKey))
		return;
	else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey))
	{
		yaw += 45.f;
		inputHandler->isMovingForward = true;
		inputHandler->isMovingRight = true;
		keyPressed = true;
	}
	else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->leftKey) &&!GameData::isKeyDown(*input->rightKey))
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

	float calcYaw = (yaw + 90) *  (PI / 180);
	float calcPitch = (gm->player->pitch)  * -(PI / 180);
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