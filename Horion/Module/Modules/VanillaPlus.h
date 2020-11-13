#pragma once
#include "Module.h"
class VanillaPlus : public IModule {

public:
	bool fastFlyingItems = true;

	VanillaPlus();
	virtual ~VanillaPlus(){};

	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onTick(C_GameMode*) override;
};
