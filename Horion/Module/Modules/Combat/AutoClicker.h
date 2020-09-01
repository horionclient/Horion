#pragma once
#include "Horion/Module/ModuleManager.h"
#include "Horion/Module/Modules/Module.h"

class AutoClicker : public IModule {
private:
	int delay = 0;
	int Odelay = 0;
	bool sword = true;
	bool rightclick = false;
	bool hold = false;

public:
	AutoClicker();
	~AutoClicker();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
