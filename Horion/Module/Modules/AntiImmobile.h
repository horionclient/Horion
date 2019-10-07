#pragma once
#include "Module.h"

class AntiImmobile : public IModule
{
public:
	C_MoveInputHandler* inputHandler = nullptr;
	AntiImmobile();
	~AntiImmobile();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};

