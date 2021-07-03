#pragma once
#include "Module.h"
class FastFall : public IModule {
public:
	FastFall();
	~FastFall();
	
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
