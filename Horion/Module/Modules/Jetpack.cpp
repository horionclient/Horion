#include "Jetpack.h"

#include "../../../Utils/Logger.h"

Jetpack::Jetpack() : IModule('F', Category::MOVEMENT, "Fly around like you had a Jetpack!") {
	registerFloatSetting("speed", &speedMod, 1, 0.2f, 3.f);
	registerBoolSetting("Bypass", &isBypass, isBypass);
}

Jetpack::~Jetpack() {
}

bool Jetpack::isFlashMode() {
	return true;
}

const char* Jetpack::getModuleName() {
	return ("Jetpack");
}

void Jetpack::onTick(C_GameMode* gm) {
	float calcYaw = (gm->player->yaw + 90) * (PI / 180);
	float calcPitch = (gm->player->pitch) * -(PI / 180);

	if (!isBypass) {
		vec3_t moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * speedMod;
		moveVec.y = sin(calcPitch) * speedMod;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * speedMod;

		gm->player->lerpMotion(moveVec);
	} else {
		delay++;

		if (delay >= 5) {
			vec3_t pos = *g_Data.getLocalPlayer()->getPos();
			C_MovePlayerPacket a(g_Data.getLocalPlayer(), pos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
			pos.y += 0.35f;
			a = C_MovePlayerPacket(g_Data.getLocalPlayer(), pos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);

			gm->player->velocity.y = 0.465f;
			vec3_t moveVec;
			moveVec.x = cos(calcYaw) * cos(calcPitch) * speedMod;
			moveVec.z = sin(calcYaw) * cos(calcPitch) * speedMod;

			gm->player->velocity.x = moveVec.x;
			gm->player->velocity.z = moveVec.z;

			float teleportX = cos(calcYaw) * cos(calcPitch) * 0.00000005f;
			float teleportZ = sin(calcYaw) * cos(calcPitch) * 0.00000005f;

			pos = *gm->player->getPos();
			g_Data.getLocalPlayer()->setPos(vec3_t(pos.x + teleportX, pos.y - 0.15f, pos.z + teleportZ));

			gm->player->velocity.y -= 0.15f;
			delay = 0;
		}
	}
}