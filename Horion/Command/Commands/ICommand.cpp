#include "ICommand.h"

ICommand::ICommand(const char * command, char * description, char * usage)
{
	this->_command = command;
	this->_description = description;
	this->_usage = usage;
	this->strCommand = std::string(command);
}

ICommand::~ICommand()
{
}
