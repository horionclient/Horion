#pragma once

#include <vector>
#include <string>
#include "../../Module/ModuleManager.h"
#include "../../../Memory/GameData.h"

class ICommand {
private:
	const char* _command;
	const char* _description;
	const char* _usage;
	std::string strCommand;
public:
	ICommand(const char* command, char* description, char* usage);
	~ICommand();

	virtual const char* getCommand() { return _command; };
	virtual std::string getCommandStr() { return strCommand; };
	virtual const char* getDescription() { return _description; };
	virtual const char* getUsage() { return _usage; };
	virtual void execute(std::vector<std::string>* args) = 0;
};