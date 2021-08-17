#pragma once
#include "..\ModuleManager.h"
#include "Module.h"

class Bhop : public IModule {
private:
	SettingEnum Bhopmode;
	float speed = 0.325f;
	float up = 0.10f;
	float Boostnum = 0.10f;
	bool lowhop = true;
	bool lowhopv2 = false;
	bool normal = false;
	bool step = true;
	bool Boost = false;

public:
	Bhop();
	~Bhop();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onMove(C_MoveInputHandler* input) override;
	virtual void onDisable() override;
};
