#pragma once
#include "Horion/Module/Modules/Module.h"
class FastEat : public IModule {
public:
	FastEat();
	~FastEat();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
