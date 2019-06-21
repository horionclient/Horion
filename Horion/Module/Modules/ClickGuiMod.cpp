#include "ClickGuiMod.h"



ClickGuiMod::ClickGuiMod() : IModule(0x0, VISUAL)
{
}


ClickGuiMod::~ClickGuiMod()
{
}

const char* ClickGuiMod::getModuleName()
{
	return ("ClickGui");
}

void ClickGuiMod::onEnable(){
	g_Data.getClientInstance()->releaseMouse();
}

void ClickGuiMod::onDisable() {
	g_Data.getClientInstance()->grabMouse();
}