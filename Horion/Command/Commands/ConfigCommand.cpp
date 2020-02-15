#include "ConfigCommand.h"

ConfigCommand::ConfigCommand() : IMCCommand("config", "Load/save configs", "<load/save> <name>") {
}

ConfigCommand::~ConfigCommand() {
}

bool ConfigCommand::execute(std::vector<std::string>* args) {
	if (args->at(1) == "load") {
		std::string name = args->at(2);
		configMgr->loadConfig(name, false);
		return true;
	} else if (args->at(1) == "save") {
		std::string name = args->at(2);
		configMgr->saveConfig(name);
		clientMessageF("[%sHorion%s] %sSuccessfully saved config %s%s%s!", GOLD, WHITE, GREEN, GRAY, name.c_str(), GREEN);
		return true;
	}
	return false;
}