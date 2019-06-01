#pragma once
#include "Module.h"

class AirJump : public IModule
{
public:
	AirJump();
	~AirJump();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};

