#pragma once
#include "Module.h"

class Killaura : public IModule
{
public:
	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual std::string getModuleName() override;
};

