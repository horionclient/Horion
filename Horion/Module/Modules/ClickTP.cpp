#include "ClickTP.h"



ClickTP::ClickTP() : IModule(0x0, EXPLOITS)
{
}


ClickTP::~ClickTP()
{

}

const char* ClickTP::getModuleName()
{
	return ("ClickTP");
}

void ClickTP::onTick(C_GameMode* gm)
{
	if (gm->player == nullptr)
		return;
	vec3_ti block = g_Data.getClientInstance()->getPointerStruct()->block;
	vec3_t pos = block.toFloatVector();
	pos.y += gm->player->height;
	pos.y += 1.f;

	if (GameData::isLeftClickDown() || GameData::isRightClickDown() && !hasClicked && GameData::canUseMoveKeys()) {
		gm->player->setPos(pos);
		g_Data.getGuiData()->displayClientMessageF("%sSuccessfully teleported!", GREEN);
		hasClicked = true;
	}
	else if (!GameData::isRightClickDown() && !GameData::isLeftClickDown()) {
		hasClicked = false;
	}
}