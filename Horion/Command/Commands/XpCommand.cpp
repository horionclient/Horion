#include "XpCommand.h"

XpCommand::XpCommand() : IMCCommand("xp", "Add experience to your player", "xp <amount>L") {
}

XpCommand::~XpCommand() {
}

bool XpCommand::execute(std::vector<std::string>* args) {
	auto player = g_Data.getLocalPlayer();
	if (!player) return false;

	std::string amount = args->at(1);
	std::transform(amount.begin(), amount.end(), amount.begin(), tolower);
	const bool levels = amount.substr(amount.length() - 1) == "l";
	if (levels) amount = amount.substr(0, amount.length() - 1);
	try {
		int xp = std::stoi(amount);
		if (levels)
			player->addLevels(xp);
		else
			player->addExperience(xp);
		clientMessageF("%sAdded %i experience %s", GREEN, xp, levels ? "levels" : "points");
	} catch (std::invalid_argument&) {
		clientMessageF("%sInvalid amount of experience!", RED);
	}
	return true;
}
