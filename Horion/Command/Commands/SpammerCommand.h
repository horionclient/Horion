#pragma once
#include "ICommand.h"
#include <sstream>

class SpammerCommand : public IMCCommand {
public:
	SpammerCommand();
	~SpammerCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
