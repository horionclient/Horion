#pragma once
#include "Horion/Module/Modules/Module.h"

class InfiniteBlockReach : public IModule {
private:
	float blockReach = 50.f;

public:
	InfiniteBlockReach();
	~InfiniteBlockReach();

	inline float getBlockReach() { return blockReach; };
	inline void setBlockReach(float reach) { blockReach = reach; };
	// Inherited via IModule
	virtual const char* getModuleName() override;
};
