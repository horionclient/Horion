#pragma once
#include "ICommand.h"
class DamageCommand : public IMCCommand {
public:
	DamageCommand();
	~DamageCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
