#pragma once
#include "Module.h"

class Aimbot : public IModule
{
public:
	Aimbot();
	~Aimbot();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender() override;
};

