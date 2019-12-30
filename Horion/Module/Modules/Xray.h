#pragma once
#include "Module.h"
class Xray : public IModule {
public:
	Xray();
	~Xray();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
