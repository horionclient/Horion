#pragma once

#include "../../Memory/GameData.h"

#include "Commands/ICommand.h"
#include "Commands/EjectCommand.h"

#include <string>
#include <vector>
#include <algorithm>

class CommandMgr {
private:
	GameData* gameData;
	std::vector<ICommand*> commandList;
public:
	CommandMgr(GameData* gm);
	~CommandMgr();

	
	void initCommands();
	void disable();
	std::vector<ICommand*>* getCommandList();

	void execute(char* message);
};

extern CommandMgr* cmdMgr;