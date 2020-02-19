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
	if (!GameData::isKeyDown(*input->forwardKey)) return;
	delay++;
	if (delay >= 2) {
		delay = 0;

		vec3_t pos = *gm->player->getPos();
		float yaw = gm->player->yaw * (PI / 180);
		float length = 3.f;

		float x = -sin(yaw) * length;
		float z = cos(yaw) * length;

		gm->player->setPos(pos.add(vec3_t(x, 0, z)));
	}
}

void TestModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
}

void TestModule::onDisable() {
}
