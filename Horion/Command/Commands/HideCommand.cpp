#include "HideCommand.h"



HideCommand::HideCommand() : IMCCommand("hide", "Hide Horion Utility Mod.", "") {
}


HideCommand::~HideCommand() {
}

bool HideCommand::execute(std::vector<std::string>* args) {
	GameData::hide();
	if (GameData::shouldHide()) {
		clientMessageF("[%sHorion%s] %sThe Horion Utility Mod has been hidden.", GOLD, WHITE, GREEN);
	}
	else {
		clientMessageF("[%sHorion%s] %sHorion Utility Mod is now visible.", GOLD, WHITE, GREEN);
	}
	return true;
}
