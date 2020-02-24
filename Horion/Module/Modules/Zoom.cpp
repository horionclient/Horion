#include "Zoom.h"

Zoom::Zoom() : IModule('C', Category::VISUAL, "Zoom in!") {
	registerFloatSetting("modifier", &this->modifier, this->modifier, 1.f, 100.f);
}

Zoom::~Zoom() {
}

bool Zoom::isFlashMode() {
	return true;
}

const char* Zoom::getModuleName() {
	return ("Zoom");
}
