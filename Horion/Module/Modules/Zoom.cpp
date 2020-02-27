#include "Zoom.h"

Zoom::Zoom() : IModule('C', Category::VISUAL, "Zoom in!") {
	registerFloatSetting("Strength", &this->strength, this->strength, 1.f, 100.f);
}

Zoom::~Zoom() {
}

bool Zoom::isFlashMode() {
	return true;
}

const char* Zoom::getModuleName() {
	return ("Zoom");
}

void Zoom::onEnable() {
	zooming = true;
}

void Zoom::onPostRender(C_MinecraftUIRenderContext* ctx) {
	target = strength;
}

void Zoom::onDisable() {
	target = g_Data.fov;
}
