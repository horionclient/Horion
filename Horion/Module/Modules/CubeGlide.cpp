#include "CubeGlide.h"

CubeGlide::CubeGlide() : IModule(0, Category::MOVEMENT, "Glide on Cubecraft") {
	registerFloatSetting("Speed", &speed, speed, 1.f, 3.f);
}

CubeGlide::~CubeGlide() {
}

const char* CubeGlide::getModuleName() {
	return "CubeGlide";
}

bool CubeGlide::isFlashMode() {
	return true;
}

void CubeGlide::onEnable() {
	g_Data.getLocalPlayer()->setPos((*g_Data.getLocalPlayer()->getPos()).add(vec3_t(0, 1, 0)));
}

void CubeGlide::onTick(C_GameMode* gm) {
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
}

void CubeGlide::onDisable() {
	g_Data.getLocalPlayer()->velocity = vec3_t(0, 0, 0);
}
