#pragma once
#include "Module.h"

class OffhandFixer : public IModule
{
public:
	OffhandFixer();
	~OffhandFixer();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
