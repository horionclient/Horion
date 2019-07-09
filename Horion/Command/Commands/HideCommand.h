#pragma once
#include "ICommand.h"
class HideCommand :
	public ICommand
{
public:
	HideCommand();
	~HideCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

