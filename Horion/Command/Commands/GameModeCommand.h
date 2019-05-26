#pragma once
#include "ICommand.h"
class GameModeCommand :
	public ICommand
{
public:
	GameModeCommand();
	~GameModeCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

