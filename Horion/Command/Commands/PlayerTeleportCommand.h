#pragma once
#include "ICommand.h"
class PlayerTeleportCommand :
	public ICommand
{
public:
	PlayerTeleportCommand();
	~PlayerTeleportCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

