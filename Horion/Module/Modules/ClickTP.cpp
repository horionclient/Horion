#include "ClickTP.h"



ClickTP::ClickTP() : IModule(0x0, Category::EXPLOITS, "Click a block to teleport to it")
{
	registerBoolSetting("Only Hand", &this->onlyHand, this->onlyHand);
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
	C_GameSettingsInput* input = g_Data.getGameSettingsInput();
	if (input == nullptr)
		return;
	if (gm->player == nullptr)
		return;
	if (onlyHand && g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot)->item != nullptr) return;
	vec3_ti block = g_Data.getClientInstance()->getPointerStruct()->block;
	if (block == vec3_ti(0, 0, 0)) return;
	vec3_t pos = block.toFloatVector();
	pos.y += gm->player->height;
	pos.y += 1.f;

	if (GameData::isRightClickDown() && !hasClicked && GameData::canUseMoveKeys()) {
		std::string coords = "X: " + std::to_string(pos.x) + " Y: " + std::to_string(pos.y) + " Z: " + std::to_string(pos.z);
		gm->player->setPos(pos);
		g_Data.getGuiData()->displayClientMessageF("%sTeleported to %s%s%s.", GREEN, GRAY, coords.c_str(), GREEN);
		hasClicked = true;
	}
	else if (!GameData::isRightClickDown()) {
		hasClicked = false;
	}
}