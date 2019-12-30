#pragma once
#include "Module.h"

class InfiniteBlockReach : public IModule {
private:
	float blockReach = 50.f;

public:
	InfiniteBlockReach();
	~InfiniteBlockReach();

	inline float getBlockReach() { return blockReach; };
	// Inherited via IModule
	virtual const char* getModuleName() override;
};
