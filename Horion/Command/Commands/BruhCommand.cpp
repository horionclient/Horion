#include "BruhCommand.h"

BruhCommand::BruhCommand() : IMCCommand("bruh", "bruh moment", "bruh") {
}

BruhCommand::~BruhCommand() {
}

bool BruhCommand::execute(std::vector<std::string>* args) {
	assertTrue(g_Data.getLocalPlayer() != nullptr);

	clientMessageF("bruh moment");
	return true;
}
