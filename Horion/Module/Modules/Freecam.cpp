#include "Freecam.h"



Freecam::Freecam() : IModule('V')
{
}


Freecam::~Freecam()
{
}

int Freecam::getFakedEditon() {
	// static const char* gameEditons[] = { "Unknown", "Android", "iOS", "macOS", "FireOS", "GearVR", "HoloLens", "Windows 10 (PC)", "Windows", "Dedicated", "Orbis", "NX" };
				
	return 1; // Android
}


std::string Freecam::getModuleName()
{
	return std::string("Freecam");
}