#pragma once

#include "../../../Memory/GameData.h"
#include <string>

class IModule
{
private:
	bool enabled = false;
	int keybind = 0x0;

protected:
	IModule(int key);

public:
	~IModule();

	virtual std::string getModuleName() = 0;
	virtual int getKeybind();
	virtual void onTick(C_GameMode*);
	virtual void onKeyUpdate(int key, bool isDown);
	virtual void onEnable();
	virtual void onDisable();
	virtual void setEnabled(bool enabled);
	virtual void toggle();
	virtual bool isEnabled();
};

