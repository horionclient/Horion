#pragma once

#include "Horion/Module/Modules/Module.h"

class Velocity : public IModule {
public:
	float xModifier = 0.f;
	float yModifier = 0.f;
	Velocity();
	~Velocity();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
