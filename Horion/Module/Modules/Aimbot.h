#pragma once
#include "Module.h"

class Aimbot : public IModule
{
public:
	Aimbot();
	~Aimbot();

	// Inherited via IModule
	virtual std::string getModuleName() override;
	virtual void onAimbot() override;
};

