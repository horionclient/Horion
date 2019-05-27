#pragma once
#include "ICommand.h"
class FriendListCommand :
	public ICommand
{
public:
	FriendListCommand();
	~FriendListCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

