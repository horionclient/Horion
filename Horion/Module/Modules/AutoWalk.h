#pragma once
#include "Module.h"

class AutoWalk : public IModule {
private:
	bool alldirections = true;
	float speed = 0.325f;

public:
	AutoWalk();
	~AutoWalk();

	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};
