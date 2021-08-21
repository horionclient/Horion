#pragma once

#include <map>

#include "ICommand.h"

class SetOffhandCommand : public IMCCommand {
public:
	SetOffhandCommand();
	~SetOffhandCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
