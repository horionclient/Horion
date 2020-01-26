#pragma once
#include "Module.h"

class CrystalAura : public IModule {
private:
	int range = 5;

public:
	CrystalAura();
	~CrystalAura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	int delay = 0;
};
