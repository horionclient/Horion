#include "CommandMgr.h"

CommandMgr::CommandMgr(GameData* gm)
{
	this->gameData = gm;
}

CommandMgr::~CommandMgr()
{
}

void CommandMgr::initCommands() {

}

void CommandMgr::disable() {

}

CommandMgr* cmdMgr = new CommandMgr(&g_Data);