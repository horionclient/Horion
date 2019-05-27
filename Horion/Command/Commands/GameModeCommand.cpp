#include "GameModeCommand.h"



GameModeCommand::GameModeCommand() : ICommand("gamemode", "Changes the player to a specific game mode", "<number>")
{
}


GameModeCommand::~GameModeCommand()
{
}


bool GameModeCommand::execute(std::vector<std::string>* args)
{
	assertTrue(g_Data.getClientInstance()->getLocalPlayer() != nullptr);
	assertTrue(args->size() >= 2);
	std::string gamemode = args->at(1);
	assertTrue(gamemode.size() > 0);

	if (gamemode.size() > 1)
	{
		clientMessageF("[%sHorion%s] %sInvalid GameMode", GOLD, WHITE, GREEN);
		return true;
	}

	int Mode = (int)gamemode.at(0);
	if (Mode >= 0x61 && Mode <= 0x7A) // Convert key to lower case
		Mode -= 0x20;
	Mode -= 0x30;
	if (Mode >= 0 && Mode <= 2)
	{
		
		g_Data.getLocalPlayer()->setGameModeType(Mode);
		g_Data.getLocalPlayer()->gamemode = Mode;
		clientMessageF("[%sHorion%s] %sGameMode changed !", GOLD, WHITE, GREEN);
		return true;
	}
	
	
	clientMessageF("[%sHorion%s] %sInvalid GameMode",GOLD,WHITE,GREEN);
	return true;
}