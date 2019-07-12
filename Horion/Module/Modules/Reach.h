#pragma once
#include "Module.h"


class Reach : public IModule
{
private:
	float reachValue = 3;
	float* reachPtr = nullptr;
public:
	Reach();
	~Reach();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};

