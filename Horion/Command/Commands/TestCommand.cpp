#include "TestCommand.h"

TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "") {
}

TestCommand::~TestCommand() {
}

void listEnts(C_Entity* ent, bool isValid) {
	int id = ent->getEntityTypeId();
	char* name = ent->getNameTag()->getText();
	logF("Entity Name: %s", name);
	logF("Entity ID: %d", id);
	logF("---------------");
}

bool TestCommand::execute(std::vector<std::string>* args) {
	return true;
}
