#pragma once
#include "Module.h"

class Phase : public IModule {
public:
	Phase();
	~Phase();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
