#pragma once
#include "Module.h"

class AntiImmobile : public IModule {
public:
	AntiImmobile();
	~AntiImmobile();

	const char* getModuleName() override;
};
