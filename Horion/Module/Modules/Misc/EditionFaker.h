#pragma once
#include "Horion/Module/Modules/Module.h"
class EditionFaker : public IModule {
public:
	EditionFaker();
	~EditionFaker();

	int getFakedEditon();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
