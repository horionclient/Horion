#pragma once
#include "ICommand.h"
class BindCommand :
	public ICommand
{
public:
	BindCommand();
	~BindCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

