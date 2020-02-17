#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class TriggerBot : public IModule {
private:
	int delay = 0;
	int Odelay = 0;
	bool sword = true;

public:
	TriggerBot();
	~TriggerBot();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
