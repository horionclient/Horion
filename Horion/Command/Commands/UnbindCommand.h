#pragma once
#include "ICommand.h"
#include <map>

class UnbindCommand :
	public ICommand
{
public:
	UnbindCommand();
	~UnbindCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

