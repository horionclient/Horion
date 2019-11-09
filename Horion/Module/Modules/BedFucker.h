#pragma once
#include "Module.h"

class BedFucker : public IModule
{
private:
	int range = 5;
	bool beds = true;
	bool eggs = true;
	bool cakes = true;
	bool treasures = true;
	bool chests = false;
	bool barrels = false;
public:
	BedFucker();
	~BedFucker();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	int delay = 0;
};

