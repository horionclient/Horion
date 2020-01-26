#pragma once
#include "ICommand.h"

class ServerCommand : public IMCCommand {
public:
	ServerCommand();
	~ServerCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
