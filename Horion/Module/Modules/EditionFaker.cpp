#include "EditionFaker.h"

EditionFaker::EditionFaker() : IModule(0, Category::MISC, "Fakes your device") {
	registerIntSetting("OS", &osValue, osValue, 0, 11);
}

EditionFaker::~EditionFaker() {
}

int EditionFaker::getFakedEditon() {
	// static const char* gameEditons[] = { "Unknown", "Android", "iOS", "macOS", "FireOS", "GearVR", "HoloLens", "Windows 10 (PC)", "Windows", "Dedicated", "Orbis", "NX" };

	return osValue;
}

const char* EditionFaker::getModuleName() {
	return ("EditionFaker");
}
