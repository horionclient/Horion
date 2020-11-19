#include "Twerk.h"

Twerk::Twerk() : IModule(0x0, Category::MISC, "Twerk it") {
	registerIntSetting("Delay", &this->delay1, 1, 0, 8);
}

Twerk::~Twerk() {
}

const char* Twerk::getModuleName() {
	return ("Twerk");
}

void Twerk::onTick(C_GameMode* gm) {
	delay++;
	g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
	if (delay >= delay1) {
		g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = true;
		delay = 0;
	}
	if (delay1 <= 1) {
		delay1 = 2;
	}
}

void Twerk::onDisable() {
	g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
}
