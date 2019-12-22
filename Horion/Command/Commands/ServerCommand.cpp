#include "ServerCommand.h"


ServerCommand::ServerCommand() : IMCCommand("server", "Show information about the server you're playing on", "") {
	registerAlias("ip");
}


ServerCommand::~ServerCommand() {
}

bool ServerCommand::execute(std::vector<std::string>* args) {
	assertTrue(g_Data.getClientInstance()->getLocalPlayer() != nullptr);
	if (!(g_Data.getRakNetInstance()->serverIp.getTextLength() < 1))
	clientMessageF("You're currently playing on:\nIP: %s\nPort: %s", g_Data.getRakNetInstance()->serverIp.getText(), std::to_string(g_Data.getRakNetInstance()->getPort()).c_str());
	else clientMessageF("%sYou're not playing on a server.", RED);
	return true;
}

