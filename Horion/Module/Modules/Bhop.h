#pragma once
#include "..\ModuleManager.h"
#include "Module.h"

class Bhop : public IModule {
private:
	float speed = 0.325f;

public:
	Bhop();
	~Bhop();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onMove(C_MoveInputHandler* input) override;
};
