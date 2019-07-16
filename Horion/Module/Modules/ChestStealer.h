#pragma once
#include "Module.h"
class ChestStealer :
	public IModule
{
public:
	C_ChestScreenController* chestScreenController = nullptr;
	ChestStealer();
	~ChestStealer();

	// Inherited via IModule
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};

