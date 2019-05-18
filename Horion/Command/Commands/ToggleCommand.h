#pragma once
#include "ICommand.h"
class ToggleCommand :
	public ICommand
{
public:
	ToggleCommand();
	~ToggleCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

