#pragma once
#include "Module.h"

class AutoWalk : public IModule {
private:
	bool sprint = false;
	bool jump = false;

public:
	SettingEnum mode;
	AutoWalk();
	~AutoWalk();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};