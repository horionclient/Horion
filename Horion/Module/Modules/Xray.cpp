#include "Xray.h"

Xray::Xray() : IModule(VK_NUMPAD6, Category::VISUAL, "See ores through walls") {
}

Xray::~Xray() {
}

const char* Xray::getModuleName() {
	return ("X-Ray");
}
