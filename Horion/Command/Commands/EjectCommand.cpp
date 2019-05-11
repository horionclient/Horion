#include "EjectCommand.h"



EjectCommand::EjectCommand() : ICommand("eject", "Removes the cheat from the game.", "")
{
}


EjectCommand::~EjectCommand()
{
}


void EjectCommand::execute(std::vector<std::string>* args)
{
	GameData::terminate();
}
