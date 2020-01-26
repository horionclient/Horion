#pragma once
#include "Module.h"
class TestModule : public IModule {
private:
public:
	TestModule();
	~TestModule();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
