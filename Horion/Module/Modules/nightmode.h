#pragma once
#include "Module.h"
class Nightmode : public IModule {
private:
public:
	Nightmode();
	~Nightmode();
	bool fdark = true;
	bool mdark = false;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};


