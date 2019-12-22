#include "ICommand.h"

IMCCommand::IMCCommand(const char * command, char * description, char * usage) {
	this->_command = command;
	this->_description = description;
	this->_usage = usage;
	registerAlias(command);
}

IMCCommand::~IMCCommand() {
}
