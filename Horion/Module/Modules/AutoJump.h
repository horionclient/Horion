#pragma once
#include "Module.h"

class AutoJump : public IModule {
public:
	AutoJump();
	~AutoJump();
	
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};