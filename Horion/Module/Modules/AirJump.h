#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class AirJump : public IModule {
private:
	int hasJumped = 0;
	bool legacyMode = false;

public:
	AirJump();
	~AirJump();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
