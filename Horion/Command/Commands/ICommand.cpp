#include "ICommand.h"

ICommand::ICommand(const char * command, char * description, char * usage)
{
	this->_command = command;
	this->_description = description;
	this->_usage = usage;
	registerAlias(command);
}

ICommand::~ICommand()
{
}
