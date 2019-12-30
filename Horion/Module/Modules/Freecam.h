#pragma once
#include "Module.h"
class Freecam : public IModule {
private:
	vec3_t oldPos;

public:
	Freecam();
	~Freecam();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};
