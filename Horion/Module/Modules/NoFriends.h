#pragma once

#include "Module.h"
#include "../../DrawUtils.h"

class NoFriends : public IModule
{
public:
	NoFriends();
	~NoFriends();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};

