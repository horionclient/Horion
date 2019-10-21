#pragma once
#include "Module.h"
#include "..\ModuleManager.h"

class Bhop : public IModule
{
private:
	bool keyPressed = false;
	float speed = 0.325f;
public:
	C_MoveInputHandler* inputHandler = nullptr;
	Bhop();
	~Bhop();

	float height = 0.325f;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};

