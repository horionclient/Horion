#pragma once
#include "Horion/Module/ModuleManager.h"
#include "Horion/Module/Modules/Module.h"
class AirJump : public IModule {
private:
	int hasJumped = 0;

public:
	AirJump();
	~AirJump();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
