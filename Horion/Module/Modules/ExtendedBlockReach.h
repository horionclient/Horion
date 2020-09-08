#pragma once
#include "Module.h"

class ExtendedBlockReach : public IModule {
private:
	float blockReach = 7.f;

public:
	ExtendedBlockReach();
	~ExtendedBlockReach();

	inline float getBlockReach() { return blockReach; };
	inline void setBlockReach(float reach) { blockReach = reach; };
	// Inherited via IModule
	virtual const char* getModuleName() override;
};
