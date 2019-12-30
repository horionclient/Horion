#pragma once
#include "ICommand.h"
class BruhCommand : public IMCCommand {
public:
	BruhCommand();
	~BruhCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
