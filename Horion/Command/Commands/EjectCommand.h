#pragma once
#include "ICommand.h"
class EjectCommand : public IMCCommand {
public:
	EjectCommand();
	~EjectCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
