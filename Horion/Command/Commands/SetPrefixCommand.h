#pragma once
#include "../CommandMgr.h"

#include "ICommand.h"
class SetPrefixCommand : public IMCCommand {
public:
	SetPrefixCommand();
	~SetPrefixCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};