#pragma once
#include "Module.h"
#include "../../DrawUtils.h"

class InfiniteReach : public IModule
{
private:
	int delay;
	float fakeX;
	float fakeZ;
public:
	InfiniteReach();
	~InfiniteReach();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};

