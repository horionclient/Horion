#pragma once
#include "ICommand.h"
class PanicCommand : public IMCCommand {
public:
	PanicCommand();
	~PanicCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
