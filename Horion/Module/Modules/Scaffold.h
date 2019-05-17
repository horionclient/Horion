#pragma once
#include "Module.h"
#include "../../DrawUtils.h"
class Scaffold :
	public IModule
{
public:
	Scaffold();
	~Scaffold();

	// Inherited via IModule
	virtual std::string getModuleName() override;
	virtual void onPostRender() override;
};

