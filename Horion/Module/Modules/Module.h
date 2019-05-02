#pragma once

#include "../../../Memory/GameData.h"
#include <string>

class IModule
{
	bool enabled = false;


public:
	IModule();
	~IModule();

	virtual std::string getModuleName() = 0;
	virtual void onTick(C_GameMode*);
	virtual void onEnable();
	virtual void onDisable();
	virtual void setEnabled(bool enabled);
	virtual bool isEnabled();
};

