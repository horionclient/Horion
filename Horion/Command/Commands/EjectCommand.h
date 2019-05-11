#pragma once
#include "ICommand.h"
class EjectCommand :
	public ICommand
{
public:
	EjectCommand();
	~EjectCommand();

	// Inherited via ICommand
	virtual void execute(std::vector<std::string>* args) override;
};

