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
	vec2_t angle = vec2_t(0, 0);
	bool hasTarget = false;
public:
	bool isMobAura = false;
	float range = 6;


	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onSendPacket(C_Packet* packet) override;
};

