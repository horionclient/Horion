#pragma once
#include <map>

#include "ICommand.h"

class BindCommand : public IMCCommand {
public:
	BindCommand();
	~BindCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
