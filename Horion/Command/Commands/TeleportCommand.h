#pragma once
#include "ICommand.h"
class TeleportCommand :
	public ICommand
{
public:
	TeleportCommand();
	~TeleportCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

