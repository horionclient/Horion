#include "Fly.h"



Fly::Fly() : IModule(0x0, MOVEMENT)
{
	this->registerBoolSetting("HurtFly", &this->hurtFly, this->hurtFly);
}


Fly::~Fly()
{
}

const char* Fly::getModuleName()
{
	return ("Fly");
}

void Fly::onEnable()
{
	if (g_Data.getLocalPlayer() != nullptr)
	{
		oldGameMode = g_Data.getLocalPlayer()->gamemode;
		g_Data.getLocalPlayer()->setGameModeType(1);
		g_Data.getLocalPlayer()->gamemode = 0;
		if (hurtFly) {
			g_Data.getLocalPlayer()->fallDistance = 4;
			g_Data.getLocalPlayer()->onGround = true;
		}
	}
}

void Fly::onDisable()
{
	if (g_Data.getLocalPlayer() != nullptr)
		g_Data.getLocalPlayer()->setGameModeType(oldGameMode);
}