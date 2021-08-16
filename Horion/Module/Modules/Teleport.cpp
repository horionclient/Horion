#include "Teleport.h"

Teleport::Teleport() : IModule(0, Category::MISC, "Click a block to teleport to it") {
	registerBoolSetting("Only Hand", &this->onlyHand, this->onlyHand);
	registerBoolSetting("Push", &this->bypass, this->bypass);
}

Teleport::~Teleport() {
}

const char* Teleport::getModuleName() {
	return "ClickTP";
}

void Teleport::onTick(C_GameMode* gm) {

	if (!GameData::canUseMoveKeys()) 
		return;
	if (onlyHand && g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot)->item != nullptr) 
		return;
	
	if (GameData::isRightClickDown() && !hasClicked) {
		hasClicked = true;

		vec3_ti block = g_Data.getClientInstance()->getPointerStruct()->block;
		if (block == vec3_ti(0, 0, 0)) return;
		vec3_t pos = block.toFloatVector();
		pos.x += 0.5f;
		pos.z += 0.5f;

		tpPos = pos;
		shouldTP = true;

		g_Data.getGuiData()->displayClientMessageF("%sTeleported!", GREEN, GRAY, pos.x, pos.y, pos.z, GREEN);
	}
	if (!GameData::isRightClickDown()) 
		hasClicked = false;

	if (shouldTP) {
		tpPos.y += (gm->player->getPos()->y - gm->player->getAABB()->lower.y) + 1;  // eye height + 1
		if (bypass) {
			float dist = gm->player->getPos()->dist(tpPos);
			g_Data.getLocalPlayer()->lerpTo(tpPos, vec2_t(1, 1), (int)fmax((int)dist * 0.1, 1));
		}
		else gm->player->setPos(tpPos);
		shouldTP = false;
	}
}
