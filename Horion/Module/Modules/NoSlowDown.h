#pragma once
#include "Module.h"
class NoSlowDown :
	public IModule
{
public:
	NoSlowDown();
	~NoSlowDown();

	// Inherited via IModule
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};

