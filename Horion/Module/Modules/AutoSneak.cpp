#include "AutoSneak.h"

AutoSneak::AutoSneak() : IModule(0x0, Category::MOVEMENT, "Automatically sneak without holding the key") {
}

AutoSneak::~AutoSneak() {
}

const char* AutoSneak::getModuleName() {
	return ("AutoSneak");
}

void AutoSneak::onTick(C_GameMode* gm) {
	auto loc = g_Data.getLocalPlayer(); // this is hella broke
	loc->setSneaking(true);

}
