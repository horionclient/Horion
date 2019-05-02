#include "Module.h"



IModule::IModule()
{
}


IModule::~IModule()
{
}

void IModule::onTick(C_GameMode*)
{
}

void IModule::onEnable()
{
}

void IModule::onDisable()
{
}

void IModule::setEnabled(bool enabled)
{
	this->enabled = enabled;
}

bool IModule::isEnabled()
{
	return this->enabled;
}
