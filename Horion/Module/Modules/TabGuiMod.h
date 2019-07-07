#pragma once
#include "Module.h"

class TabGuiMod :
	public IModule
{
public:
	TabGuiMod();
	~TabGuiMod();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};

