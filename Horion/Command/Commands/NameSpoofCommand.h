#pragma once
#include "ICommand.h"
#include <sstream>

class NameSpoofCommand : public IMCCommand {
public:
	NameSpoofCommand();
	~NameSpoofCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
