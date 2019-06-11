#include "HelpCommand.h"

#include "../CommandMgr.h"

HelpCommand::HelpCommand() : ICommand("help", "Lists all commands", "")
{
	registerAlias("h");
}


HelpCommand::~HelpCommand()
{
}

bool HelpCommand::execute(std::vector<std::string>* args)
{
	g_Data.getGuiData()->displayClientMessageF("==========");
	std::vector<ICommand*>* commandList = cmdMgr->getCommandList();
	for (auto it = commandList->begin(); it != commandList->end(); ++it) {
		ICommand* plump = *it;
		g_Data.getGuiData()->displayClientMessageF("%s%s - %s%s", plump->getCommand(), GRAY, ITALIC, plump->getDescription());
	}
	return true;
}
