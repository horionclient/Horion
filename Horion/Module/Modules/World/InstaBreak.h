#pragma once
#include "Horion/Module/Modules/Module.h"

class InstaBreak : public IModule {
public:
	InstaBreak();
	~InstaBreak();
	// Inherited via IModule
	virtual const char* getModuleName() override;
};
