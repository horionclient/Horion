#include "EjectCommand.h"

EjectCommand::EjectCommand() : IMCCommand("eject", "Removes the cheat from the game.", "") {
	registerAlias("uninject");
}

EjectCommand::~EjectCommand() {
}

bool EjectCommand::execute(std::vector<std::string>* args) {
	GameData::terminate();
	return true;
}
