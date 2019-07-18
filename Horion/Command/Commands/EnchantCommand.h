#pragma once
#include "ICommand.h"
class EnchantCommand :
	public IMCCommand
{
public:
	EnchantCommand();
	~EnchantCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};

