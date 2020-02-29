#include "TestModule.h"

TestModule::TestModule() : IModule(0, Category::EXPLOITS, "For testing purposes") {
}

TestModule::~TestModule() {
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

bool TestModule::isFlashMode() {
	return false;
}

void TestModule::onEnable() {
}

void TestModule::onTick(C_GameMode* gm) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (input == nullptr)
		return;
 
	float calcYaw = (gm->player->yaw + 90) * (PI / 180);
	vec3_t moveVec;
	moveVec.x = cos(calcYaw) * 1.f;
	moveVec.y = gm->player->velocity.y;
	moveVec.z = sin(calcYaw) * 1.f;
	if (GameData::isKeyDown(*input->forwardKey)) {
		gm->player->lerpMotion(moveVec);
		if (gm->player->onGround == true && GameData::canUseMoveKeys() && gm->player->velocity.squaredxzlen() > 0.001f) {
			gm->player->velocity.x *= 1.39f;
			gm->player->velocity.z *= 1.39f;
			gm->player->velocity.y = 0.035f;
		}
	}
}

void TestModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
}

void TestModule::onDisable() {
}
