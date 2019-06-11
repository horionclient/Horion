#pragma once
#include "ICommand.h"
class EnchantCommand :
	public ICommand
{
public:
	EnchantCommand();
	~EnchantCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

