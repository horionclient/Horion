#pragma once
#include "Module.h"
class AutoStaircase : public IModule {
private:
	//from scaffold:
	bool spoof = false;

public:
	AutoStaircase();
	~AutoStaircase();
	bool findBlock();
	bool tryScaffold(vec3_t blockBelow);

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};