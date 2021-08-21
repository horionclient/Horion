#pragma once

#include <map>

#include "ICommand.h"

class setoffhandCommand : public IMCCommand {
public:
	setoffhandCommand();
	~setoffhandCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
