#pragma once
#include "Module.h"

class BoatFly : public IModule {
private:
	float boatflyspeed = 1.f;
public:
	BoatFly();
	~BoatFly();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};