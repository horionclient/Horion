#include "SetprefixCommand.h"
#include "../CommandMgr.h"
#include "../../Config/ConfigManager.h"
#include <string>

SetprefixCommand::SetprefixCommand() : IMCCommand("setprefix", "Set Horion's command prefix", "<prefix> (has to be one character)")
{
	registerAlias("prefix");
}


SetprefixCommand::~SetprefixCommand()
{
}

bool SetprefixCommand::execute(std::vector<std::string>* args)
{
	if (args->size() > 1 && args->at(1).length() == 1) {
		int prefix = args->at(1).at(0);
		cmdMgr->prefix = prefix;
		configMgr->saveConfig();
		clientMessageF("%sThe prefix has been set to %s%s%s", GREEN, BOLD, GRAY, args->at(1).c_str());
		return true;
	}
	return false;
}