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
	if (key == getKeybind()) {
		if (isFlashMode())
			setEnabled(isDown);
		else if (isDown)
			toggle();
	}
		
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

bool IModule::isFlashMode()
{
	return false;
}


void IModule::setEnabled(bool enabled)
{
	if (this->enabled != enabled) {
		this->enabled = enabled;
#ifndef _DEBUG
		if(!isFlashMode()) // Only print jetpack stuff in debug mode
#endif
			logF("%s %s", enabled ? "Enabled" : "Disabled", this->getModuleName().c_str());

		if (enabled)
			this->onEnable();
		else
			this->onDisable();
	}
}

void IModule::toggle()
{
	setEnabled(!this->enabled);
}

bool IModule::isEnabled()
{
	return this->enabled;
}
