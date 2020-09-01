#pragma once
#include "Horion/Module/Modules/Module.h"
class InventoryMove : public IModule {
private:
	bool keyPressed = false;

public:
	InventoryMove();
	~InventoryMove();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
