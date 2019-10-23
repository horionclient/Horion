#pragma once
#include "Module.h"
class ChestStealer :
	public IModule
{
private:
	int delay = 0;
	int setDelay = 7;
public:
	C_ChestScreenController* chestScreenController = nullptr;
	ChestStealer();
	~ChestStealer();

	// Inherited via IModule
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};

