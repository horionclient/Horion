#pragma once
#include "Module.h"

class ForceCreative : public IModule
{
private:
	bool oneTime = true;
	int oldGameMode;
public:
	ForceCreative();
	~ForceCreative();

	virtual std::string getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
