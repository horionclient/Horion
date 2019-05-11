#pragma once

#include "../../Memory/GameData.h"

class CommandMgr {
private:
	GameData* gameData;
public:
	CommandMgr(GameData* gm);
	~CommandMgr();

	void initCommands();
	void disable();
	
};

extern CommandMgr* cmdMgr;