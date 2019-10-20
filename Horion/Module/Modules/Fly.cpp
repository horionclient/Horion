#include "Fly.h"



Fly::Fly() : IModule(0x0, Category::MOVEMENT, "Enables fly like in creative mode")
{
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
	}
}

void Fly::onDisable()
{
	if (g_Data.getLocalPlayer() != nullptr)
		g_Data.getLocalPlayer()->setGameModeType(oldGameMode);
}