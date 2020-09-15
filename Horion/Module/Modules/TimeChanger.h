#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class TimeChanger : public IModule {
public:
	TimeChanger();
	~TimeChanger();

	float modifier = 0.5f;

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
