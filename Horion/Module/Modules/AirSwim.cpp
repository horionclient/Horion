#include "AirSwim.h"



AirSwim::AirSwim() : IModule(0x0, MOVEMENT)
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
	g_Data.getLocalPlayer()->didEnterWaterBool = true;
	g_Data.getLocalPlayer()->doWaterSplashEffect();
}
