#pragma once
#include "Module.h"
#include "../ModuleManager.h"
#include "../../../Utils/Target.h"

class Killaura : public IModule
{
private:
	bool isMulti = true;
	int delay = 0;
	int Odelay = 0;
	bool autoweapon = false;
	void findWeapon();
public:
	bool isMobAura = false;
	float range = 6;
	vec2_t angle = vec2_t(0, 0);
	bool hasTarget = false;

	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
};

