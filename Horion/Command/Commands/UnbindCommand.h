#pragma once
#include "ICommand.h"
#include <map>

class UnbindCommand :
	public IMCCommand
{
public:
	UnbindCommand();
	~UnbindCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};

