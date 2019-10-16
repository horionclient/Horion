#include "HudModule.h"

HudModule::HudModule() : IModule(0x0, VISUAL)
{
	registerBoolSetting("TabGui", &this->tabgui, this->tabgui);
	registerBoolSetting("ArrayList", &this->arraylist, this->arraylist);
	registerBoolSetting("Watermark", &this->watermark, this->watermark);
}


HudModule::~HudModule()
{
}

const char* HudModule::getModuleName()
{
	return ("HUD");
}