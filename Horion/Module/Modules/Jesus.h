#pragma once
#include "Module.h"
class Jesus :
	public IModule
{
private:
	bool wasInWater = false;
	bool isSneaking = false;
public:
	Jesus();
	~Jesus();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onMove(C_MoveInputHandler* input) override;
	virtual void onTick(C_GameMode* gm) override;
};

