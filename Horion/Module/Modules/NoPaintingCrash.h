#pragma once
#include "Module.h"
class NoPaintingCrash : public IModule {
public:
	NoPaintingCrash();
	~NoPaintingCrash();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};