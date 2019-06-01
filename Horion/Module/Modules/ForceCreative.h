#pragma once
#include "Module.h"

class ForceCreative : public IModule
{
private:
	int oldGameMode = 0;
public:
	ForceCreative();
	~ForceCreative();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
