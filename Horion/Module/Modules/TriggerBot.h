#pragma once
#include "Module.h"

class TriggerBot : public IModule
{
public:
	TriggerBot();
	~TriggerBot();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};

