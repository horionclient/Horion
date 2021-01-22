#pragma once
#include "Module.h"
class TestModule : public IModule {
private:
	float playersize = 1;
	bool active = false;
	float ogsize = 0;

public:
	
	TestModule();
	~TestModule();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
