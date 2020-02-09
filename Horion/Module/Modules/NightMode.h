#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class NightMode : public IModule {
public:
	NightMode();
	~NightMode();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
};
