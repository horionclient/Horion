#pragma once
#include "Module.h"

class OffhandFixer : public IModule
{
private:
	int itemId = 513;
	char count = 1;
	char itemData = 0;
public:
	OffhandFixer();
	~OffhandFixer();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
