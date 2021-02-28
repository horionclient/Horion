#include "Fly.h"

#define FlyMode 0
#define CubeMode 1

Fly::Fly() : IModule(0, Category::MOVEMENT, "Enables fly like in creative mode") {
	mode = SettingEnum(this,
							{
								{"Fly", 1},
								{"Cube glide", 2}
							});
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
		case CubeMode:
			return true;
		default:
			return false;
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
	case FlyMode:
		gm->player->canFly = true;
		break;
	case CubeMode:
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
		case FlyMode:
			if (g_Data.getLocalPlayer() != nullptr)
				if (g_Data.getLocalPlayer()->gamemode != 1)
					g_Data.getLocalPlayer()->canFly = false;
			break;
		case CubeMode:
			g_Data.getLocalPlayer()->velocity = vec3_t(0, 0, 0);
			break;
	}
}
