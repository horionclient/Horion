#include "AirSwim.h"



AirSwim::AirSwim() : IModule(0x0, MOVEMENT, "Swim even though your in air")
{
}


AirSwim::~AirSwim()
{
}

const char* AirSwim::getModuleName()
{
	return ("AirSwim");
}

void AirSwim::onEnable()
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	g_Data.getLocalPlayer()->didEnterWaterBool = true;
	g_Data.getLocalPlayer()->doWaterSplashEffect();
}
