#pragma once
#include "..\ModuleManager.h"
#include "Module.h"

class Strafe : public IModule {
private:
	float speed = 0.325f;

public:
	Strafe();
	~Strafe();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onMove(C_MoveInputHandler* input) override;
};
