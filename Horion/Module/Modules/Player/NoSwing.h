#pragma once
#include "Horion/Module/Modules/Module.h"
class NoSwing : public IModule {
public:
	NoSwing();
	~NoSwing();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
