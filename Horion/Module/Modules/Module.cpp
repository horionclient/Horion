#include "Module.h"



IModule::IModule(int key)
{
	this->keybind = key;
}

IModule::~IModule()
{
}

int IModule::getKeybind()
{
	return this->keybind;
}

void IModule::onTick(C_GameMode*)
{
}
void IModule::onAimbot()
{
}
void IModule::onTrigger()
{
}
void IModule::onKeyUpdate(int key, bool isDown)
{
	if (isDown && key == getKeybind())
		toggle();
}

void IModule::onEnable()
{
}

void IModule::onDisable()
{
}

void IModule::onPreRender()
{
}

void IModule::onPostRender()
{
}


void IModule::setEnabled(bool enabled)
{
	this->enabled = enabled;
	logF("%s %s", enabled ? "Enabled" : "Disabled", this->getModuleName().c_str());
}

void IModule::toggle()
{
	setEnabled(!this->enabled);
}

bool IModule::isEnabled()
{
	return this->enabled;
}
