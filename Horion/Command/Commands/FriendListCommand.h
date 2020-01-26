#pragma once
#include "ICommand.h"
class FriendListCommand : public IMCCommand {
public:
	FriendListCommand();
	~FriendListCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
