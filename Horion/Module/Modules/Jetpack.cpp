#include "Jetpack.h"

Jetpack::Jetpack() : IModule('F', Category::MOVEMENT, "Fly around like you had a Jetpack!") {
	registerFloatSetting("speed", &this->speedMod, 1, 0.2f, 3.f);
	this->registerBoolSetting("Bypass", &this->isBypass, this->isBypass);
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

	if (isBypass) {
		vec3_t pos = *g_Data.getLocalPlayer()->getPos();
		pos.y += 1.f;
		C_MovePlayerPacket* a = new C_MovePlayerPacket(g_Data.getLocalPlayer(), pos);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(a);
		delete a;
		pos.y -= 1.f;
		C_MovePlayerPacket* a2 = new C_MovePlayerPacket(g_Data.getLocalPlayer(), pos);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(a2);
		delete a2;
	}

	vec3_t moveVec;
	moveVec.x = cos(calcYaw) * cos(calcPitch) * speedMod;
	moveVec.y = sin(calcPitch) * speedMod;
	moveVec.z = sin(calcYaw) * cos(calcPitch) * speedMod;

	gm->player->lerpMotion(moveVec);
}
