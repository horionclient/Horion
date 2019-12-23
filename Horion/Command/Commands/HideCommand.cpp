#include "HideCommand.h"



HideCommand::HideCommand() : IMCCommand("hide", "Hides Horion.", "") {
}


HideCommand::~HideCommand() {
}

bool HideCommand::execute(std::vector<std::string>* args) {
	GameData::hide();
	if (GameData::shouldHide()) {
		clientMessageF("[%sHorion%s] %sHorion has been hidden.", GOLD, WHITE, GREEN);
	}
	else {
		clientMessageF("[%sHorion%s] %sHorion is now visible.", GOLD, WHITE, GREEN);
	}
	return true;
}
