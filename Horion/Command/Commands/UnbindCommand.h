#pragma once
#include <map>

#include "ICommand.h"

class UnbindCommand : public IMCCommand {
public:
	UnbindCommand();
	~UnbindCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
