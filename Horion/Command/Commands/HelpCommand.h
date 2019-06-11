#pragma once
#include "ICommand.h"
class HelpCommand :
	public ICommand
{
public:
	HelpCommand();
	~HelpCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

