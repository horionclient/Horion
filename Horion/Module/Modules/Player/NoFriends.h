#pragma once

#include "Horion/DrawUtils.h"
#include "Horion/Module/Modules/Module.h"

class NoFriends : public IModule {
public:
	NoFriends();
	~NoFriends();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
