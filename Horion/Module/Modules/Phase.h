#pragma once
#include "Module.h"

class Phase : public IModule
{
public:
	Phase();
	~Phase();

	virtual std::string getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};

