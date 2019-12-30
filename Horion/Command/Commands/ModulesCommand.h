#pragma once
#include "ICommand.h"
class ModulesCommand : public IMCCommand {
public:
	ModulesCommand();
	~ModulesCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
