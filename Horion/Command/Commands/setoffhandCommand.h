#pragma once

#include "ICommand.h"
#include <map>

class setoffhandCommand :
	public IMCCommand
{
public:
	setoffhandCommand();
	~setoffhandCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};

