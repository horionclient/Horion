#pragma once
#include "Module.h"
class AutoSprint :
	public IModule
{
public:
	AutoSprint();
	~AutoSprint();
	
	virtual void onTick(C_GameMode* gm) override;
	virtual std::string getModuleName() override;
};

