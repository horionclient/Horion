#pragma once
#include "Module.h"
class Tracer :
	public IModule
{
public:
	Tracer();
	~Tracer();

	// Inherited via IModule
	virtual std::string getModuleName() override;
};

