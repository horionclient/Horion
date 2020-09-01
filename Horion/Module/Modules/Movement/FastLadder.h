#pragma once
#include "Horion/Module/Modules/Module.h"
class FastLadder : public IModule {
public:
	FastLadder();
	~FastLadder();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
