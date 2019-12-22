#include "TestCommand.h"


TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "") {
}


TestCommand::~TestCommand() {
}

bool TestCommand::execute(std::vector<std::string>* args) {
	return true;
}