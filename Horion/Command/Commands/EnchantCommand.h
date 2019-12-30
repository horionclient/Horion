#pragma once
#include "ICommand.h"
class EnchantCommand : public IMCCommand {
private:
	std::map<std::string, int> enchantMap;

public:
	EnchantCommand();
	~EnchantCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
