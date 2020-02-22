#include "NameSpoofCommand.h"

NameSpoofCommand::NameSpoofCommand() : IMCCommand("namespoof", "Spoof your name on realms/bds", "<name/reset> <name>") {
	registerAlias("ns");
}

NameSpoofCommand::~NameSpoofCommand() {
}

bool NameSpoofCommand::execute(std::vector<std::string>* args) {
	if (args->at(1) == "name" && args->at(2) != "") {
		g_Data.fakeName->setText(args->at(2));
		clientMessageF("[%sHorion%s] %sSet fakename to %s%s%s, please reconnect!", GOLD, WHITE, GREEN, GRAY, args->at(2).c_str(), GREEN);
		return true;
	} else if (args->at(1) == "reset") {
		g_Data.fakeName->resetWithoutDelete();
		clientMessageF("[%sHorion%s] %sReset fakename!", GOLD, WHITE, GREEN);
		return true;
	}
	return false;
}
