#pragma once
#include "ICommand.h"
class GiveCommand : public IMCCommand {
public:
	GiveCommand();
	~GiveCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
