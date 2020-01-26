#pragma once
#include "Module.h"
class Fly : public IModule {
public:
	Fly();
	~Fly();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
