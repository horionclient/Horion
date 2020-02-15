#pragma once
#include "ICommand.h"
#include "..\..\Config\ConfigManager.h"

class ConfigCommand : public IMCCommand {
public:
	ConfigCommand();
	~ConfigCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};