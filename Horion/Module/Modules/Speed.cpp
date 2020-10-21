#include "Speed.h"

Speed::Speed() : IModule(VK_NUMPAD2, Category::MOVEMENT, "Speed up!") {
	registerFloatSetting("speed", &this->speed, 1, 0.1f, 2.f);
	registerFloatSetting("boost", &this->boost, 4.10f, 3.50f, 5.0f);
	registerFloatSetting("timer", &this->timerspeed, 20.0f, 20.0f, 40.0f);
	registerBoolSetting("motion", &this->motion, false);
	registerBoolSetting("custom", &this->custom, true);
	registerBoolSetting("use Timer", &this->timer, false);
}

Speed::~Speed() {
}

const char* Speed::getModuleName() {
	return ("Speed");  // 48 8D 15 ?? ?? ?? ?? 48 8B CB FF 90 ?? ?? ?? ?? 48 8B D8
}

void Speed::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (timer == true) {
		*g_Data.getClientInstance()->minecraft->timer = this->timerspeed;
	}

	if (custom == true) {
		float* speedAdr = reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84);
		*speedAdr = speed;
		if (localPlayer->onGround == true && GameData::canUseMoveKeys() && localPlayer->velocity.squaredxzlen() > 0.001f) {
			localPlayer->velocity.x *= 1.39f;
			localPlayer->velocity.z *= 1.39f;
			localPlayer->velocity.y = 0.084f;
		}

	} else if (motion == true) {
		stage++;

		if (localPlayer->onGround == true) {
			if (stage == 1) {
				localPlayer->velocity.x *= boost;
				localPlayer->velocity.z *= boost;
				//stage = 1;
			}
			if (stage == 2) {
				localPlayer->velocity.x *= 0.793f;
				localPlayer->velocity.z *= 0.793f;
				//stage = 2;
			}
			if (stage == 3) {
				localPlayer->velocity.x *= 0.697f;
				localPlayer->velocity.z *= 0.697f;
				//stage = 3;
			}
			if (stage == 4) {
				stage = 0;
			}
		} else {
			stage = 2;
		}
	}
}

void Speed::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) {
		this->setEnabled(false);
		return;
	} else {
		origSpeed = *reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84);
	}
}

void Speed::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr)
		*reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84) = origSpeed;
	*g_Data.getClientInstance()->minecraft->timer = 20.0f;
}
