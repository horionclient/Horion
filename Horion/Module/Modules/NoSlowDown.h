#pragma once
#include "Module.h"
class NoSlowDown :
	public IModule
{
public:
	NoSlowDown();
	~NoSlowDown();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};

