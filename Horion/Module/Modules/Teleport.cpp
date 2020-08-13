#include "Teleport.h"

Teleport::Teleport() : IModule(0x0, Category::MISC, "Click a block to teleport to it") {
	registerBoolSetting("Only Hand", &this->onlyHand, this->onlyHand);
	registerBoolSetting("Push", &this->bypass, this->bypass);
}

Teleport::~Teleport() {
}

const char* Teleport::getModuleName() {
	return "Teleport";
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

		g_Data.getGuiData()->displayClientMessageF("%sTeleport position set to %sX: %.1f Y: %.1f Z: %.1f%s. Sneak to teleport!", GREEN, GRAY, pos.x, pos.y, pos.z, GREEN);
	}
	if (!GameData::isRightClickDown()) 
		hasClicked = false;

	if (shouldTP && gm->player->isSneaking()) {
		tpPos.y += (gm->player->getPos()->y - gm->player->getAABB()->lower.y) + 1;  // eye height + 1
		if (bypass) {
			/*int dist = (int)gm->player->getPos()->dist(tpPos);
			int i = (int)dist / 5;
			for (int n = 0; n < i; n++) {
				vec3_t offs = tpPos.sub(*gm->player->getPos()).div(i).mul(n);
				C_MovePlayerPacket p = C_MovePlayerPacket(g_Data.getLocalPlayer(), gm->player->getPos()->add(offs));
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
			}
			gm->player->setPos(tpPos);*/
			float dist = gm->player->getPos()->dist(tpPos);
			g_Data.getLocalPlayer()->lerpTo(tpPos, vec2_t(1, 1), (int)fmax((int)dist * 0.1, 1));
		}
		else gm->player->setPos(tpPos);
		shouldTP = false;
	}
}
