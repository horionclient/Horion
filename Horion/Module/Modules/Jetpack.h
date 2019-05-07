#pragma once
#include "Module.h"


class Jetpack :
	public IModule
{
public:
	Jetpack();
	~Jetpack();

	virtual void onTick(C_GameMode* gm) override;
	virtual bool isFlashMode() override;

	// Inherited via IModule
	virtual std::string getModuleName() override;
};

