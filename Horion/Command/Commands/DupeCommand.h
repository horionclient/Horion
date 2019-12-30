#pragma once
#include "ICommand.h"

class DupeCommand : public IMCCommand {
public:
	DupeCommand();
	~DupeCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
