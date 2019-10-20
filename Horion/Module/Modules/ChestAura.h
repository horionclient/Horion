#pragma once
#include "Module.h"
#include "..\ModuleManager.h"

class ChestAura : public IModule
{
public:
	C_MoveInputHandler* inputHandler = nullptr;
	ChestAura();
	~ChestAura();

	std::list<vec3_ti> chestlist;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};