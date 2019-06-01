#pragma once
#include "Module.h"


class Jetpack :
	public IModule
{
private:
	float speedMod = 1;
public:
	Jetpack();
	~Jetpack();

	virtual void onTick(C_GameMode* gm) override;
	virtual bool isFlashMode() override;

	// Inherited via IModule
	virtual const char* getModuleName() override;
};

