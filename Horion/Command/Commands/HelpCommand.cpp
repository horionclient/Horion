#include "HelpCommand.h"

#include "../CommandMgr.h"

HelpCommand::HelpCommand() : IMCCommand("help", "Lists all commands", "") {
	registerAlias("h");
}

HelpCommand::~HelpCommand() {
}

bool HelpCommand::execute(std::vector<std::string>* args) {
	g_Data.getGuiData()->displayClientMessageF("==========");
	std::vector<IMCCommand*>* commandList = cmdMgr->getCommandList();
	for (auto it = commandList->begin(); it != commandList->end(); ++it) {
		IMCCommand* plump = *it;
		g_Data.getGuiData()->displayClientMessageF("%s%s - %s%s", plump->getCommand(), GRAY, ITALIC, plump->getDescription());
	}
	return true;
}
