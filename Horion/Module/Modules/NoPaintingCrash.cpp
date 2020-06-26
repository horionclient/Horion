#include "NoPaintingCrash.h"

NoPaintingCrash::NoPaintingCrash() : IModule(0x0, Category::MISC, "Fix Painting crash when using mob spawner !") {
}

NoPaintingCrash::~NoPaintingCrash() {
}

const char* NoPaintingCrash::getModuleName() {
	return ("NoPaintingCrash");
}