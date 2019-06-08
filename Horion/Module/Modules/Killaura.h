#pragma once
#include "Module.h"
#include "../../../Utils/Target.h"

class Killaura : public IModule
{
public:
	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
};

