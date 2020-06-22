#include "AirSwim.h"

AirSwim::AirSwim() : IModule(0x0, Category::MOVEMENT, "Swim even though your in air") {
}

AirSwim::~AirSwim() {
}

const char* AirSwim::getModuleName() {
	return ("AirSwim");
}

void AirSwim::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	g_Data.getLocalPlayer()->didEnterWaterBool = true;
	g_Data.getLocalPlayer()->startSwimming();
}

void AirSwim::onTick(C_GameMode* gm) {
	g_Data.getLocalPlayer()->didEnterWaterBool = true;
	g_Data.getLocalPlayer()->doWaterSplashEffect();
	g_Data.getLocalPlayer()->startSwimming();
}

void AirSwim::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	g_Data.getLocalPlayer()->didEnterWaterBool = false;
	g_Data.getLocalPlayer()->stopSwimming();
}