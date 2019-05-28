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

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	
	if (localPlayer == nullptr)
		return;

	if (localPlayer->onGround == true) {
		localPlayer->velocity.x *= 1.39;
		localPlayer->velocity.z *= 1.39;
		localPlayer->velocity.y = 0.035;
	}
}

void Speed::onEnable()
{
	if (g_Data.getLocalPlayer() == nullptr) {
		this->setEnabled(false);
		return;
	}
	
}

void Speed::onDisable()
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	
}
