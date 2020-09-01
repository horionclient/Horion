#pragma once
#include "Horion/Module/Modules/Module.h"
class ChestStealer : public IModule {
private:
	int delay = 0;
	int setDelay = 7;
	bool enhanced = true;

public:
	ChestStealer();
	~ChestStealer();

	// Inherited via IModule
	virtual void chestScreenController_tick(C_ChestScreenController* c);
	virtual const char* getModuleName() override;
};
