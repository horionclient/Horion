#include "AutoClicker.h"

AutoClicker::AutoClicker() : IModule(0x0, Category::COMBAT, "A simple autoclicker, automatically clicks for you.") {
	this->registerBoolSetting("rightclick", &this->rightclick, rightclick);
	this->registerIntSetting("delay", &this->delay, this->delay, 0, 20);
	this->registerBoolSetting("only swords/axes", &this->sword, this->sword);
}

AutoClicker::~AutoClicker() {
}

const char* AutoClicker::getModuleName() {
	return ("AutoClicker");
}

void AutoClicker::onTick(C_GameMode* gm) {
	if (!GameData::isLeftClickDown() && Odelay != 0) Odelay = 0;
	if (GameData::isLeftClickDown() && GameData::canUseMoveKeys()) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		C_Entity* target = g_Data.getClientInstance()->getPointerStruct()->entityPtr;
		Odelay++;

		if (Odelay >= delay) {
			auto selectedItemId = localPlayer->getSelectedItemId();
			if (sword && !(selectedItemId == 268 || selectedItemId == 267 || selectedItemId == 272 || selectedItemId == 276 || selectedItemId == 283 /*swords*/
						   || selectedItemId == 271 || selectedItemId == 275 || selectedItemId == 279 || selectedItemId == 286 || selectedItemId == 258 /*axes*/))
				return;

			localPlayer->swingArm();

			if (target != 0)
				gm->attack(target);
			Odelay = 0;
		}
	}

	if (rightclick) {
		if (!GameData::isRightClickDown() && Odelay != 0) Odelay = 0;
		if (GameData::isRightClickDown() && GameData::canUseMoveKeys()) {
			PointingStruct* pstruct = g_Data.getClientInstance()->getPointerStruct();
			Odelay++;
			if (Odelay >= delay) {
				gm->buildBlock(new vec3_ti(pstruct->block), pstruct->blockSide);
				Odelay = 0;
			}
		}
	}
}
