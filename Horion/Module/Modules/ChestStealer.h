#pragma once
#include "Module.h"
class ChestStealer :
	public IModule
{
public:
	ChestStealer();
	~ChestStealer();

	// Inherited via IModule
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};

