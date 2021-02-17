#include "Twerk.h"

Twerk::Twerk() : IModule(0x0, Category::MOVEMENT, "Crouch Spam") {
	registerFloatSetting("delay", &this->delay, this->delay, 0.05f, 10);
}

Twerk::~Twerk() {
}

const char* Twerk::getModuleName() {
	return ("Twerk");
}

void Twerk::onTick(C_GameMode* gm) {
	Odelay++;
	if (Odelay > delay * 20) {
		C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
		if (gm->player->isSneaking()) {
			isSneaking = false;
		} 
		else {
			if (!gm->player->isSneaking()) {
				isSneaking = true;
			}
		}
		g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = isSneaking;
		Odelay = 0;
	}
}