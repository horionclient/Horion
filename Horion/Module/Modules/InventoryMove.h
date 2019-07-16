#pragma once
#include "Module.h"
class InventoryMove :
	public IModule
{
public:
	InventoryMove();
	~InventoryMove();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};

