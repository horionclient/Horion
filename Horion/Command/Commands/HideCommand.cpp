#include "HideCommand.h"

HideCommand::HideCommand() : IMCCommand("hide", "hide the cheat", "") {
}

HideCommand::~HideCommand() {
}

bool HideCommand::execute(std::vector<std::string>* args) {
	GameData::hide();
	if (GameData::shouldHide()) {
		clientMessageF("[%sHorion%s] %sCheat Hidden", GOLD, WHITE, GREEN);
	} else {
		clientMessageF("[%sHorion%s] %sCheat is now visible", GOLD, WHITE, GREEN);
	}
	return true;
}
