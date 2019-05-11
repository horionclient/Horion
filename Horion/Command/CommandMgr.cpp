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
			if (c->getCommandStr() == cmd) {
				c->execute(args);
			}
		}

		delete args;
	}
}

CommandMgr* cmdMgr = new CommandMgr(&g_Data);