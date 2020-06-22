#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class Fucker : public IModule {
private:
	int range = 5;
	bool beds = true;
	bool eggs = true;
	bool cakes = true;
	bool treasures = true;
	bool chests = false;
	bool barrels = false;

public:
	Fucker();
	~Fucker();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
