#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class FightBot : public IModule {
private:
	int Odelay = 0;
	float range = 6.f;
	float reach = 4.f;
	int delay = 0;

public:
	FightBot();
	~FightBot();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onDisable() override;
};
