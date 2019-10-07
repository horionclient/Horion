#include "SpamCommand.h"
#include <random>
#include <string>

SpamCommand::SpamCommand() : IMCCommand("spam", "Spams a given message", "<count> <message>")
{
}

SpamCommand::~SpamCommand()
{
}

bool SpamCommand::execute(std::vector<std::string>* args)
{
	assertTrue(args->size() > 2);

	int times = std::stoi(args->at(1));

	std::ostringstream os;
	for (int i = 2; i < args->size(); i++) {
		if (i > 1)
			os << " ";
		os << args->at(i);
	}
	for (int i = 0; i < times; i++) {
		C_TextPacket* textPacket = new C_TextPacket();
		textPacket->message.setText(os.str() + " | " + random_string());
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(textPacket);
	}
	return true;
}

std::string SpamCommand::random_string()
{
	std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

	std::random_device rd;
	std::mt19937 generator(rd());

	std::shuffle(str.begin(), str.end(), generator);

	return str.substr(0, 8);
}