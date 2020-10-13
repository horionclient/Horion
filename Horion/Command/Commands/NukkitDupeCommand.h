#pragma once
#include "ICommand.h"

class NukkitDupeCommand : public IMCCommand {
public:
	NukkitDupeCommand();
	~NukkitDupeCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
