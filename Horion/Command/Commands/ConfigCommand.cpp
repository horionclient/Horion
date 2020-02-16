#include "ConfigCommand.h"

ConfigCommand::ConfigCommand() : IMCCommand("config", "Load/save configs", "<load/new/save> <name>") {
}

ConfigCommand::~ConfigCommand() {
}

bool ConfigCommand::execute(std::vector<std::string>* args) {
	if (args->at(1) == "load") {
		std::string name = args->at(2);
		configMgr->loadConfig(name, false);
		return true;
	} else if (args->at(1) == "new") {
		std::string name = args->at(2);
		configMgr->loadConfig(name, true);
		return true;
	} else if (args->at(1) == "save") {
		configMgr->saveConfig();
		clientMessageF("[%sHorion%s] %sSuccessfully saved config %s%s%s!", GOLD, WHITE, GREEN, GRAY, configMgr->currentConfig.c_str(), GREEN);
		return true;
	}
	return false;
}