#pragma once
#include "ICommand.h"

class TestCommand : public IMCCommand {
public:
	TestCommand();
	~TestCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
