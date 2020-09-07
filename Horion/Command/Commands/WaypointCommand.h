#pragma once

#include "ICommand.h"
#include "../../Module/ModuleManager.h"
#include "../../Module/Modules/Waypoints.h"

class WaypointCommand : public IMCCommand {
public:
	WaypointCommand();
	~WaypointCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
