#pragma once
#include "Module.h"
class NightMode : public IModule {
private:
public:
	NightMode();
	~NightMode();
	bool fdark = true;
	bool mdark = false;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
