#pragma once
#include "Module.h"
class EditionFaker :
	public IModule
{
public:
	EditionFaker();
	~EditionFaker();

	int getFakedEditon();

	// Inherited via IModule
	virtual std::string getModuleName() override;
};

