#pragma once
#include "..\ModuleManager.h"
#include "Module.h"

class BugUp : public IModule {
private:
	int distance = 5;
	vec3_t savepos;

public:
	C_MoveInputHandler* inputHandler = nullptr;
	BugUp();
	~BugUp();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
