#pragma once
#include "Module.h"

class BedFucker : public IModule
{
public:
	BedFucker();
	~BedFucker();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	int delay = 0;
};

