#pragma once
#include "Module.h"
class NoWeb : public IModule {
public:
	NoWeb();
	~NoWeb();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
