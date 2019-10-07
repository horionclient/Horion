#pragma once
#include "ICommand.h"
class SpamCommand :
	public IMCCommand
{
public:
	SpamCommand();
	~SpamCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;

private:
	virtual std::string random_string();
};

