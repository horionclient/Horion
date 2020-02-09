#include "ClickTP.h"

ClickTP::ClickTP() : IModule(0x0, Category::EXPLOITS, "Click a block to teleport to it") {
	registerBoolSetting("Only Hand", &this->onlyHand, this->onlyHand);
	registerBoolSetting("Bypass", &this->bypass, this->bypass);
}

ClickTP::~ClickTP() {
}

const char* ClickTP::getModuleName() {
	return "ClickTP";
}

void ClickTP::onTick(C_GameMode* gm) {
	if (gm->player == nullptr)
		return;
	if (onlyHand && g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot)->item != nullptr)
		return;
	vec3_ti block = g_Data.getClientInstance()->getPointerStruct()->block;
	if (block == vec3_ti(0, 0, 0))
		return;
	vec3_t pos = block.toFloatVector();

	if (GameData::isRightClickDown() && !hasClicked && GameData::canUseMoveKeys()) {
		g_Data.getGuiData()->displayClientMessageF("%sTeleported to %sX: %.1f Y: %.1f Z: %.1f%s.", GREEN, GRAY, pos.x, pos.y, pos.z, GREEN);
		pos.y += (gm->player->getPos()->y - gm->player->getAABB()->lower.y) + 1;  // eye height + 1
		pos.x += 0.5f;
		pos.z += 0.5f;
		if (bypass) {
			vec3_t posNew = pos;
			C_MovePlayerPacket* a = new C_MovePlayerPacket(g_Data.getLocalPlayer(), posNew);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(a);
			delete a;
			posNew.y += 1.5f;
			gm->player->setPos(posNew);
			C_MovePlayerPacket* a2 = new C_MovePlayerPacket(g_Data.getLocalPlayer(), posNew);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(a2);
			delete a2;
			posNew.y -= 2.f;
			gm->player->setPos(posNew);
			C_MovePlayerPacket* a3 = new C_MovePlayerPacket(g_Data.getLocalPlayer(), posNew);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(a3);
			delete a3;
		}
		gm->player->setPos(pos);
		hasClicked = true;
	} else if (!GameData::isRightClickDown()) {
		hasClicked = false;
	}
}
