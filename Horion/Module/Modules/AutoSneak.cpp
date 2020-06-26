#include "AutoSneak.h"

AutoSneak::AutoSneak() : IModule(0x0, Category::MOVEMENT, "Automatically sneak without holding the key") {
}

AutoSneak::~AutoSneak() {
}

const char* AutoSneak::getModuleName() {
	return ("AutoSneak");
}

void AutoSneak::onTick(C_GameMode* gm) {
	g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = true;
}

void AutoSneak::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr)
		g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
}
