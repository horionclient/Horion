#include "ServerCommand.h"


ServerCommand::ServerCommand() : IMCCommand("server", "Show information about the server you're playing on", "")
{
}


ServerCommand::~ServerCommand()
{
}

bool ServerCommand::execute(std::vector<std::string>* args)
{
	assertTrue(g_Data.getClientInstance()->getLocalPlayer() != nullptr);
	clientMessageF("You're currently playing on: %s", g_Data.getRakNetInstance()->serverIp.getText());
	return true;
}

