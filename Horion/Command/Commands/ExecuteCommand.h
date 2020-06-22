#pragma once

#include "ICommand.h"

#include "../../Scripting/ScriptManager.h"

class ExecuteCommand : public IMCCommand {
public:
	ExecuteCommand();
	~ExecuteCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
