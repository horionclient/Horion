#pragma once
#include "Horion/Module/Modules/Module.h"

class AntiImmobile : public IModule {
public:
	AntiImmobile();
	~AntiImmobile();

	const char* getModuleName() override;
};
