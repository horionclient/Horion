#pragma once
#include "Module.h"
class Teleport : public IModule {
private:
	bool hasClicked = false;
	bool onlyHand = false;
	bool bypass = false;
	bool shouldTP = false;
	vec3_t tpPos;

public:
	Teleport();
	~Teleport();

	// Inherited via IModule
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};
