#pragma once
#include "ICommand.h"
class PanicCommand :
	public ICommand
{
public:
	PanicCommand();
	~PanicCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

