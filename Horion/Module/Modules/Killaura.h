#pragma once
#include "Module.h"

class Killaura : public IModule
{
private:
	int delay;
public:
	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual std::string getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
};

