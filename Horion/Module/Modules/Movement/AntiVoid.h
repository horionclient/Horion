#pragma once
#include "Horion/Module/ModuleManager.h"
#include "Horion/Module/Modules/Module.h"

class AntiVoid : public IModule {
private:
	int distance = 5;
	vec3_t savepos;

public:
	C_MoveInputHandler* inputHandler = nullptr;
	AntiVoid();
	~AntiVoid();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
