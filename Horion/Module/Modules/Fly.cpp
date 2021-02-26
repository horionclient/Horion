#include "Fly.h"

Fly::Fly() : IModule(0, Category::MOVEMENT, "Enables fly like in creative mode") {
	mode = (*new SettingEnum(this)).addEntry(EnumEntry("Fly", 1)).addEntry(EnumEntry("CubeGlide", 2));
	registerEnumSetting("Mode", &mode, 0);
	registerFloatSetting("CubeGlide Speed", &this->speed, this->speed, 1.f, 3.f);
}

Fly::~Fly() {
}



const char* Fly::getModuleName() {
	return ("Fly");
}

bool Fly::isFlashMode() {
	switch (mode.selected) {
	case 0:
		return false;
		break;
	case 1:
		return true;
		break;
	}
}

void Fly::onEnable() {
	if (mode.selected == 1) {
		g_Data.getLocalPlayer()->setPos((*g_Data.getLocalPlayer()->getPos()).add(vec3_t(0, 1, 0)));
	}
}


void Fly::onTick(C_GameMode* gm) {
	gm->player->canFly = true;

	switch (mode.selected) {
	case 0:
		gm->player->canFly = true;
		break;
	case 1:
		float calcYaw = (gm->player->yaw + 90) * (PI / 180);

		gameTick++;

		vec3_t pos = *g_Data.getLocalPlayer()->getPos();
		pos.y += 1.3f;
		C_MovePlayerPacket a(g_Data.getLocalPlayer(), pos);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
		pos.y -= 1.3f;
		C_MovePlayerPacket a2(g_Data.getLocalPlayer(), pos);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a2);

		vec3_t moveVec;
		moveVec.x = cos(calcYaw) * speed;
		moveVec.z = sin(calcYaw) * speed;

		gm->player->lerpMotion(moveVec);

		if (gameTick >= 5) {
			gameTick = 0;
			float yaw = gm->player->yaw * (PI / 180);
			float length = 4.f;

			float x = -sin(yaw) * length;
			float z = cos(yaw) * length;

			gm->player->setPos(pos.add(vec3_t(x, 0.5f, z)));
		}
		break;
	}

}

void Fly::onDisable() {
	switch (mode.selected) {
	case 0:
		if (g_Data.getLocalPlayer() != nullptr)
			if (g_Data.getLocalPlayer()->gamemode != 1)
				g_Data.getLocalPlayer()->canFly = false;
		break;
	case 1:
		g_Data.getLocalPlayer()->velocity = vec3_t(0, 0, 0);
	}
}
