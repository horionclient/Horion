#pragma once
#include "Module.h"

class GameMode : public IModule
{
private:
	bool oneTime = true;
	int oldGameMode;
public:
	GameMode();
	~GameMode();

	virtual std::string getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};

