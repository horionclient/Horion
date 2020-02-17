#pragma once
#include "Module.h"
class NoSwing : public IModule {
public:
	NoSwing();
	~NoSwing();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
