#pragma once
#include "Module.h"

class HudModule :
	public IModule
{
public:
	HudModule();
	~HudModule();

	bool tabgui = true;
	bool arraylist = true;
	bool watermark = true;
	bool alwaysShow = true;
	bool coordinates = false;

	// Inherited via IModule
	virtual const char* getModuleName() override;
};