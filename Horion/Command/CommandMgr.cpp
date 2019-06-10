#include "CommandMgr.h"

CommandMgr::CommandMgr(GameData* gm)
{
	this->gameData = gm;
}

CommandMgr::~CommandMgr()
{
}

void CommandMgr::initCommands() {
	commandList.push_back(new EjectCommand());
	commandList.push_back(new TeleportCommand());
	commandList.push_back(new BindCommand());
	commandList.push_back(new RelativeTeleportCommand());
	commandList.push_back(new ToggleCommand());
	commandList.push_back(new PlayerTeleportCommand());
	commandList.push_back(new GameModeCommand());
	commandList.push_back(new FriendListCommand());
	commandList.push_back(new UnbindCommand());
#ifdef _DEBUG
	commandList.push_back(new TestCommand());
#endif
}

void CommandMgr::disable() {

}

std::vector<ICommand*>* CommandMgr::getCommandList()
{
	return &commandList;
}

void CommandMgr::execute(char * message)
{
	if (message != nullptr) {
		std::vector<std::string>* args = new std::vector<std::string>(); // Stolen from https://stackoverflow.com/questions/5888022/split-string-by-single-spaces
		std::string msgStr = message + 1;
		size_t pos = msgStr.find(" "), initialPos = 0;
		while (pos != std::string::npos) {
			args->push_back(msgStr.substr(initialPos, pos - initialPos));
			initialPos = pos + 1;

			pos = msgStr.find(" ", initialPos);
		}
		args->push_back(msgStr.substr(initialPos, min(pos, msgStr.size()) - initialPos + 1));

		std::string cmd = ((*args)[0]);
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

		
		for (std::vector<ICommand*>::iterator it = this->commandList.begin(); it != this->commandList.end(); ++it) {
			ICommand* c = *it;
			auto* aliases = c->getAliasList();
			for (std::vector<std::string>::iterator it = aliases->begin(); it != aliases->end(); ++it) {
				if (*it == cmd) {
					try {
						if (!c->execute(args))
							g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s%sUsage: %s.%s %s", RED, BOLD, RESET, c->getCommand(), c->getUsage());
					}
					catch (...) {
						g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s%sUsage: %s.%s %s", RED, BOLD, RESET, c->getCommand(), c->getUsage());
					}
					goto done;
				}
			}
			
		}

		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[%sHorion%s] %sCommand '%s' could not be found!", GOLD, WHITE, RED, cmd.c_str());

	done:

		delete args;
	}
}

CommandMgr* cmdMgr = new CommandMgr(&g_Data);