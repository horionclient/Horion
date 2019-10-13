#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class AutoTotem : public IModule
{
private:
	int delay = 0;
	int tickDelay = 3;
	bool isSOH = false;
public:
	AutoTotem();
	~AutoTotem();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
};

