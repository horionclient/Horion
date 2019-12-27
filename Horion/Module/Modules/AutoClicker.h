#pragma once
#include "Module.h"

class AutoClicker : public IModule {
private:
	int delay = 0;
	int Odelay = 0;
	bool sword = true;
	bool rightclick = false;

public:
	AutoClicker();
	~AutoClicker();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
