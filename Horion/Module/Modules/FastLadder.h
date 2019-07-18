#pragma once
#include "Module.h"
class FastLadder :
	public IModule
{
private:
	float* opcode = 0;
public:
	FastLadder();
	~FastLadder();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};

