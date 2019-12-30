#pragma once
#include "ICommand.h"
class HideCommand : public IMCCommand {
public:
	HideCommand();
	~HideCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
