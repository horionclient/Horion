#pragma once
#include "ICommand.h"
class ToggleCommand : public IMCCommand {
public:
	ToggleCommand();
	~ToggleCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
