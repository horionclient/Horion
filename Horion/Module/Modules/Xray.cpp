#include "Xray.h"



Xray::Xray() : IModule(VK_NUMPAD6, Category::VISUAL, "X-Ray view!") {
}


Xray::~Xray() {
}

const char* Xray::getModuleName() {
	return ("X-Ray");
}