#include "Speed.h"



Speed::Speed() : IModule(VK_NUMPAD2, Category::MOVEMENT, "Speed up!")
{
	registerFloatSetting("speed", &this->speed, 1, 0.1f, 3.f);
}


Speed::~Speed()
{
}

const char* Speed::getModuleName()
{
	return ("Speed"); // 48 8D 15 ?? ?? ?? ?? 48 8B CB FF 90 ?? ?? ?? ?? 48 8B D8
}

void Speed::onTick(C_GameMode * gm)
{

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	
	if (localPlayer == nullptr)
		return;
	float* speedAdr = reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed()+0x84);
	*speedAdr = speed;

	if (localPlayer->onGround == true && GameData::canUseMoveKeys() && localPlayer->velocity.squaredxzlen() > 0.001f) {
		localPlayer->velocity.x *= 1.39f;
		localPlayer->velocity.z *= 1.39f;
		localPlayer->velocity.y = 0.035f;
	}
}

void Speed::onEnable()
{
	if (g_Data.getLocalPlayer() == nullptr) {
		this->setEnabled(false);
		return;
	}
	else {
		origSpeed = *reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed()+0x84);
	}
	
}

void Speed::onDisable()
{
	if (g_Data.getLocalPlayer() != nullptr)
		*reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84) = origSpeed;
}
