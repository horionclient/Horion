#include "TestModule.h"
#include "../../../resource.h"

TestModule::TestModule() : IModule(0, Category::EXPLOITS, "For testing purposes") {
}

TestModule::~TestModule() {
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

void TestModule::onTick(C_GameMode* gm) {
}

void TestModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	vec2_t pos(200.5, 23.5);
	float imageHeight = 100.f;
	float imageWidth = 120.f;
	vec2_t idk(80.f, 100.f);
	std::string filePath = "textures/entity/fireworks";
	DrawUtils::drawImage(filePath, pos, vec2_t(imageWidth, imageHeight), idk);
}

void TestModule::onDisable() {
}
