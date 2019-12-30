#pragma once
#include "Module.h"

class AutoGapple : public IModule {
private:
	int health;

public:
	C_MoveInputHandler* inputHandler = nullptr;
	AutoGapple();
	~AutoGapple();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
