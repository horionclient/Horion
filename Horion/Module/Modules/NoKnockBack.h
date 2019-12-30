#pragma once

#include "Module.h"

class NoKnockBack : public IModule {
public:
	float xModifier = 0.f;
	float yModifier = 0.f;
	NoKnockBack();
	~NoKnockBack();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
