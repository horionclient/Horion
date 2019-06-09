#pragma once
#include "Module.h"
#include "../../../Utils/Target.h"

class MobAura : public IModule
{
public:
	MobAura();
	~MobAura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
};

