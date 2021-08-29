#pragma once
#include "Module.h"
class Step : public IModule {
private:
	float height = 2.f;

public:
	Step();
	~Step();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_Player* player) override;
	virtual void onDisable() override;
};
