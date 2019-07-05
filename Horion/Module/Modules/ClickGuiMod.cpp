#include "ClickGuiMod.h"



ClickGuiMod::ClickGuiMod() : IModule(VK_INSERT, VISUAL)
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

bool ClickGuiMod::allowAutoStart()
{
	return false;
}

void ClickGuiMod::onDisable() {
	g_Data.getClientInstance()->grabMouse();
}

void ClickGuiMod::onPostRender() {
	if (GameData::canUseMoveKeys())
		g_Data.getClientInstance()->releaseMouse();
}