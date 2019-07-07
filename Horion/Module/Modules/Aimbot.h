#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class Aimbot : public IModule
{
private:
	float range = 6;
	float smoothNess = 7;
public:
	Aimbot();
	~Aimbot();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender() override;
};

