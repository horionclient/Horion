#pragma once
#include "Horion/Module/Modules/Module.h"
class RainbowSky : public IModule {
public:
	RainbowSky();
	~RainbowSky();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
