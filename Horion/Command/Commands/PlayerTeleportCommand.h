#pragma once
#include "ICommand.h"
class PlayerTeleportCommand : public IMCCommand {
public:
	PlayerTeleportCommand();
	~PlayerTeleportCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
