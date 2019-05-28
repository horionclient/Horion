#include "Speed.h"



Speed::Speed() : IModule(VK_NUMPAD2)
{
}


Speed::~Speed()
{
}

std::string Speed::getModuleName()
{
	return std::string("Speed"); // 48 8D 15 ?? ?? ?? ?? 48 8B CB FF 90 ?? ?? ?? ?? 48 8B D8
}

void Speed::onTick(C_GameMode * gm)
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	if (g_Data.getLocalPlayer()->onGround == true) {
		g_Data.getLocalPlayer()->velocity.x *= 1.2;
		g_Data.getLocalPlayer()->velocity.z *= 1.2;
	}
}

void Speed::onEnable()
{
	if (g_Data.getLocalPlayer() == nullptr) {
		this->setEnabled(false);
		return;
	}
	origSpeed = g_Data.getLocalPlayer()->getSpeed();
}

void Speed::onDisable()
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	
}
