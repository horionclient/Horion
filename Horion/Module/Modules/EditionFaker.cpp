#include "EditionFaker.h"



EditionFaker::EditionFaker() : IModule('X', EXPLOITS)
{
}


EditionFaker::~EditionFaker()
{
}

int EditionFaker::getFakedEditon() {
	// static const char* gameEditons[] = { "Unknown", "Android", "iOS", "macOS", "FireOS", "GearVR", "HoloLens", "Windows 10 (PC)", "Windows", "Dedicated", "Orbis", "NX" };
				
	return 1; // Android
}


const char* EditionFaker::getModuleName()
{
	return ("EditionFaker");
}