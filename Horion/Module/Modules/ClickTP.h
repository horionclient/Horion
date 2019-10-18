#pragma once
#include "Module.h"
class ClickTP :
	public IModule
{
private:
	bool hasClicked = false;
	bool onlyHand = false;
public:
	C_ChestScreenController* chestScreenController = nullptr;
	ClickTP();
	~ClickTP();

	// Inherited via IModule
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};