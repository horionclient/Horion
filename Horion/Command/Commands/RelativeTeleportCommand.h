#pragma once
#include "ICommand.h"
class RelativeTeleportCommand :
	public ICommand
{
public:
	RelativeTeleportCommand();
	~RelativeTeleportCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

