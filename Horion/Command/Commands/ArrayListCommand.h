#pragma once
#include "ICommand.h"
class ArrayListCommand :
	public ICommand
{
public:
	ArrayListCommand();
	~ArrayListCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

