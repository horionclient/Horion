#include "TriggerBot.h"
#include "../../../Utils/Target.h"



TriggerBot::TriggerBot() : IModule('K', Category::COMBAT, "Attacks entities you're looking at") {
	this->registerIntSetting("delay", &this->delay, this->delay, 0, 20);
}


TriggerBot::~TriggerBot() {
}

const char* TriggerBot::getModuleName() {
	return ("TriggerBot");
}
void TriggerBot::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	C_Entity* target = g_Data.getClientInstance()->getPointerStruct()->entityPtr;
	Odelay++;
	if (target != 0 && Odelay >= delay) {
		if (!Target::isValidTarget(target))
			return;

		localPlayer->swingArm();
		gm->attack(target);

		Odelay = 0;
	}
}

