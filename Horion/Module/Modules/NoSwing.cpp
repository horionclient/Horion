#include "NoSwing.h"

NoSwing::NoSwing() : IModule(0, Category::PLAYER, "Disable arm swing animation for killaura, triggerbot, fucker etc.") {
}

NoSwing::~NoSwing() {
}

const char* NoSwing::getModuleName() {
	return ("NoSwing");
}