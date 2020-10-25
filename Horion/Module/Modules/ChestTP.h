#pragma once
#include "Module.h"
class ChestTP : public IModule {
private:
	int range = 20;

public:
	std::vector<vec3_ti> chestlist;

	ChestTP();
	~ChestTP();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
