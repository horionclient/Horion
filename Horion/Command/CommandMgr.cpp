#include "CommandMgr.h"

CommandMgr::CommandMgr(GameData* gm) {
	this->gameData = gm;
}

CommandMgr::~CommandMgr() {
	for (int i = 0; i < commandList.size(); i++) {
		delete commandList[i];
		commandList[i] = nullptr;
	}
}

void CommandMgr::initCommands() {
	logF("Initializing commands");

	commandList.push_back(new EjectCommand());
	commandList.push_back(new TeleportCommand());
	commandList.push_back(new BindCommand());
	commandList.push_back(new RelativeTeleportCommand());
	commandList.push_back(new ToggleCommand());
	commandList.push_back(new PlayerTeleportCommand());
	commandList.push_back(new GameModeCommand());
	commandList.push_back(new FriendListCommand());
	commandList.push_back(new UnbindCommand());
	commandList.push_back(new EnchantCommand());
	commandList.push_back(new HelpCommand());
	commandList.push_back(new ModulesCommand());
	commandList.push_back(new PanicCommand());
	commandList.push_back(new HideCommand());
	commandList.push_back(new GiveCommand());
	commandList.push_back(new BruhCommand());
	commandList.push_back(new ServerCommand());
	commandList.push_back(new setoffhandCommand());
	commandList.push_back(new CoordsCommand());
	commandList.push_back(new SayCommand());
	commandList.push_back(new SpammerCommand());
	commandList.push_back(new DupeCommand());
	commandList.push_back(new DamageCommand());
	commandList.push_back(new ConfigCommand());
	commandList.push_back(new SetprefixCommand());
	commandList.push_back(new NbtCommand());

	commandList.push_back(new CommandBlockExploitCommand());
	commandList.push_back(new NameSpoofCommand());
	commandList.push_back(new ExecuteCommand());
	commandList.push_back(new ScriptCommand());
	commandList.push_back(new PathCommand());

#ifdef _DEBUG
	commandList.push_back(new TestCommand());
#endif
}

void CommandMgr::disable() {
}

std::vector<IMCCommand*>* CommandMgr::getCommandList() {
	return &commandList;
}

void CommandMgr::execute(char* message) {
	if (message != nullptr) {
		std::vector<std::string> args;
		std::string msgStr = message + 1;
		size_t pos = msgStr.find(" "), initialPos = 0;
		while (pos != std::string::npos) {
			args.push_back(msgStr.substr(initialPos, pos - initialPos));
			initialPos = pos + 1;

			pos = msgStr.find(" ", initialPos);
		}
		args.push_back(msgStr.substr(initialPos, min(pos, msgStr.size()) - initialPos + 1));

		std::string cmd = args[0];
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

		for (auto it = this->commandList.begin(); it != this->commandList.end(); ++it) {
			IMCCommand* c = *it;
			auto* aliases = c->getAliasList();
			for (auto it = aliases->begin(); it != aliases->end(); ++it) {
				if (*it == cmd) {
					try {
						if (!c->execute(&args))
							g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s%sUsage: %s%c%s %s", RED, BOLD, RESET, cmdMgr->prefix, c->getCommand(), c->getUsage(cmd.c_str()));
					} catch (...) {
						g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s%sUsage: %s%c%s %s", RED, BOLD, RESET, cmdMgr->prefix, c->getCommand(), c->getUsage(cmd.c_str()));
					}
					return;
				}
			}
		}

		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[%sHorion%s] %sCommand '%s' could not be found!", GOLD, WHITE, RED, cmd.c_str());
	}
}

CommandMgr* cmdMgr = new CommandMgr(&g_Data);
