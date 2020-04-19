#pragma once
#include "../../Scripting/ScriptManager.h"
#include "ICommand.h"

class ScriptCommand : public IMCCommand {
public:
	ScriptCommand();
	~ScriptCommand();

	virtual bool execute(std::vector<std::string>* args) override;
};
