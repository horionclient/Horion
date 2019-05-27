#pragma once
#include "Module.h"


class Nuker :
	public IModule
{
public:
	Nuker();
	~Nuker();
	// Inherited via IModule
	virtual std::string getModuleName() override;
};

