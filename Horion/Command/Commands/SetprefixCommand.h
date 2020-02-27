#pragma once
#include "../CommandMgr.h"

#include "ICommand.h"
class SetprefixCommand : public IMCCommand {
public:
	SetprefixCommand();
	~SetprefixCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
