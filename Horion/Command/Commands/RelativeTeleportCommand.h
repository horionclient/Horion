#pragma once
#include "ICommand.h"
class RelativeTeleportCommand : public IMCCommand {
public:
	RelativeTeleportCommand();
	~RelativeTeleportCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
