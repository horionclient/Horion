#include "AutoSneak.h"

AutoSneak::AutoSneak() : IModule(0x0, Category::MOVEMENT, "Automatically sneak without holding the key") {
}

AutoSneak::~AutoSneak() {
}

const char* AutoSneak::getModuleName() {
	return ("AutoSneak");
}

void AutoSneak::onMove(C_MoveInputHandler* handler) {
	handler->isSneakDown = true;
}
