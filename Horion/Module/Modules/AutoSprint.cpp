#include "AutoSprint.h"

AutoSprint::AutoSprint() : IModule(0, Category::MOVEMENT, "Automatically sprint without holding the key") {
	registerBoolSetting("all directions", &this->alldirections, this->alldirections);
}

AutoSprint::~AutoSprint() {
}

const char* AutoSprint::getModuleName() {
	return ("AutoSprint");
}

void AutoSprint::onTick(C_Player* plr) {
	if (!plr->isSprinting() && plr->velocity.magnitudexz() > 0.01f) {
		C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
		if (alldirections || GameData::isKeyDown(*input->forwardKey))
			plr->setSprinting(true);
	}
}