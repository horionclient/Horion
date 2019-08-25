#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class Aimbot : public IModule
{
private:
	float range = 4;
	bool vertical = true;
	bool sword = true;
	bool click = true;
	int horizontalspeed = 50;
	int verticalspeed = 50;
	int verticalrange = 40;
	int horizontalrange = 60;
public:
	Aimbot();
	~Aimbot();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender() override;
};

