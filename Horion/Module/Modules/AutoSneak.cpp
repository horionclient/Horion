#include "AutoSneak.h"

AutoSneak::AutoSneak() : IModule(0x0, Category::MOVEMENT, "Automatically sneak without holding the key") {
}


AutoSneak::~AutoSneak() {
}

const char* AutoSneak::getModuleName() {
	return ("AutoSneak");
}

const char* AutoSneak::getRawModuleName() {
	return ("AutoSneak_");
}

void AutoSneak::onTick(C_GameMode* gm) {
	if (gm->player != nullptr && !gm->player->isSneaking()) {
		gm->player->setSneaking(true);

	}

}