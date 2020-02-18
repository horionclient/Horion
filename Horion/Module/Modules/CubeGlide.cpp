#include "CubeGlide.h"

CubeGlide::CubeGlide() : IModule(0, Category::MOVEMENT, "Glide on Cubecraft") {
	registerFloatSetting("Speed", &this->speed, this->speed, 1.f, 3.f);
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

	vec3_t pos = *g_Data.getLocalPlayer()->getPos();
	pos.y += 1.1f;
	C_MovePlayerPacket* a = new C_MovePlayerPacket(g_Data.getLocalPlayer(), pos);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(a);
	delete a;
	pos.y -= 1.2f;
	C_MovePlayerPacket* a2 = new C_MovePlayerPacket(g_Data.getLocalPlayer(), pos);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(a2);
	delete a2;

	vec3_t moveVec;
	moveVec.x = cos(calcYaw) * speed;
	moveVec.z = sin(calcYaw) * speed;

	gm->player->lerpMotion(moveVec);
}

void CubeGlide::onDisable() {
	g_Data.getLocalPlayer()->velocity = vec3_t(0, 0, 0);
}
