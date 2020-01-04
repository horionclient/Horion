#include "ICommand.h"

IMCCommand::IMCCommand(const char* command, const char* description, const char* usage) {
	this->_command = command;
	this->_description = description;
	this->_usage = usage;
	registerAlias(command);
}

IMCCommand::~IMCCommand() {
}
