#include "HudModule.h"

HudModule::HudModule() : IModule(0x0, Category::VISUAL, "Displays ArrayList/TabGUI")
{
	registerBoolSetting("TabGui", &this->tabgui, this->tabgui);
	registerBoolSetting("ArrayList", &this->arraylist, this->arraylist);
	registerBoolSetting("Watermark", &this->watermark, this->watermark);
	registerBoolSetting("Coordinates", &this->coordinates, this->coordinates);
	// registerBoolSetting("Always Show", &this->alwaysShow, this->alwaysShow); Disabled due to crashes
}

HudModule::~HudModule()
{
}

const char* HudModule::getModuleName()
{
	return ("HUD");
}