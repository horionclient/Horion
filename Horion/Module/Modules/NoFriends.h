#pragma once

#include "../../DrawUtils.h"
#include "Module.h"

class NoFriends : public IModule {
public:
	NoFriends();
	~NoFriends();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
