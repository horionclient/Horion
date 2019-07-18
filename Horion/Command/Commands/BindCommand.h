#pragma once
#include "ICommand.h"
#include <map>

class BindCommand :
	public IMCCommand
{
public:
	BindCommand();
	~BindCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};

