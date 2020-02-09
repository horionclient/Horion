#pragma once
#include "Module.h"
class ClickTP : public IModule {
private:
	bool hasClicked = false;
	bool onlyHand = false;
	bool bypass = false;
	bool shouldTP = false;
	vec3_t tpPos;

public:
	ClickTP();
	~ClickTP();

	// Inherited via IModule
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};
