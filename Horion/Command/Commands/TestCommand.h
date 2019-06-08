#pragma once
#include "ICommand.h"
class TestCommand :
	public ICommand
{
public:
	TestCommand();
	~TestCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

