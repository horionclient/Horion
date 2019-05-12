#pragma once
#include "Module.h"
class Freecam :
	public IModule
{
public:
	Freecam();
	~Freecam();

	int getFakedEditon();

	// Inherited via IModule
	virtual std::string getModuleName() override;
};

