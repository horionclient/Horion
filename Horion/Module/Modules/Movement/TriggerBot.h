#pragma once
#include "Horion/Module/ModuleManager.h"
#include "Horion/Module/Modules/Module.h"

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
