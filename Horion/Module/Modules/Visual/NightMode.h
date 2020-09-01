#pragma once
#include "Horion/Module/ModuleManager.h"
#include "Horion/Module/Modules/Module.h"

class NightMode : public IModule {
public:
	NightMode();
	~NightMode();

	float modifier = 0.5f;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
};
