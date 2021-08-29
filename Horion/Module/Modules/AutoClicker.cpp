#include "AutoClicker.h"

AutoClicker::AutoClicker() : IModule(0, Category::COMBAT, "A simple autoclicker, automatically clicks for you.") {
	this->registerBoolSetting("rightclick", &this->rightclick, rightclick);
	this->registerBoolSetting("only weapons", &this->weapons, this->weapons);
	this->registerBoolSetting("break blocks", &this->breakBlocks, this->breakBlocks);
	this->registerIntSetting("delay", &this->delay, this->delay, 0, 20);
	this->registerBoolSetting("hold", &this->hold, this->hold);
}

AutoClicker::~AutoClicker() {
}

const char* AutoClicker::getModuleName() {
	return ("AutoClicker");
}

void AutoClicker::onTick(C_Player* plr) {
	if ((GameData::isLeftClickDown() || !hold) && GameData::canUseMoveKeys()) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		PointingStruct* pointing = g_Data.getClientInstance()->getPointerStruct();
		Odelay++;

		if (Odelay >= delay) {
			auto selectedItem = localPlayer->getSelectedItem();
			if (weapons && selectedItem->getAttackingDamageWithEnchants() < 1)
				return;

			g_Data.leftclickCount++;

			localPlayer->swingArm();

			if (pointing->hasEntity() != 0)
				plr->getGm()->attack(pointing->getEntity());
			else if (breakBlocks) {
				bool isDestroyed = false;
				plr->getGm()->startDestroyBlock(pointing->block, pointing->blockSide, isDestroyed);
				plr->getGm()->stopDestroyBlock(pointing->block);
				if (isDestroyed && localPlayer->region->getBlock(pointing->block)->blockLegacy->blockId != 0)
					plr->getGm()->destroyBlock(&pointing->block, 0);
			}
			Odelay = 0;
		}
	}

	if (rightclick) {
		if ((GameData::isRightClickDown() || !hold) && GameData::canUseMoveKeys()) {
			PointingStruct* pstruct = g_Data.getClientInstance()->getPointerStruct();
			Odelay++;
			if (Odelay >= delay) {
				g_Data.rightclickCount++;
				plr->getGm()->buildBlock(new vec3_ti(pstruct->block), pstruct->blockSide);
				Odelay = 0;
			}
		}
	}
}