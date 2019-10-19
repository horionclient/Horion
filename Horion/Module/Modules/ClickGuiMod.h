#pragma once
#include "Module.h"

class ClickGuiMod :
	public IModule
{
public:
	ClickGuiMod();
	~ClickGuiMod();

	bool showTooltips = true;

	// Inherited via IModule
	virtual void onPostRender() override;
	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	virtual bool allowAutoStart() override;
};

