#pragma once
#include "ICommand.h"
class ModulesCommand :
	public ICommand
{
public:
	ModulesCommand();
	~ModulesCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

