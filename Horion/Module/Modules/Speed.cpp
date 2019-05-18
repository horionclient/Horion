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
	g_Data.getLocalPlayer()->setSpeed(5);
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
	g_Data.getLocalPlayer()->setSpeed(origSpeed);
}
