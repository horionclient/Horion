#include "AutoClicker.h"

AutoClicker::AutoClicker() : IModule(0, Category::COMBAT, "A simple autoclicker, automatically clicks for you.") {
	registerBoolSetting("rightclick", &rightclick, rightclick);
	registerBoolSetting("only weapons", &weapons,weapons);
	registerBoolSetting("break blocks", &breakBlocks, breakBlocks);
	registerIntSetting("delay", &delay, delay, 0, 20);
	registerBoolSetting("hold", &hold, hold);
}

AutoClicker::~AutoClicker() {
}

const char* AutoClicker::getModuleName() {
	return ("AutoClicker");
}

void AutoClicker::onTick(C_GameMode* gm) {
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
				gm->attack(pointing->getEntity());
			else if (breakBlocks) {
				bool isDestroyed = false;
				gm->startDestroyBlock(pointing->block, pointing->blockSide, isDestroyed);
				gm->stopDestroyBlock(pointing->block);
				if (isDestroyed && localPlayer->region->getBlock(pointing->block)->blockLegacy->blockId != 0)
					gm->destroyBlock(&pointing->block, 0);
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
				gm->buildBlock(new vec3_ti(pstruct->block), pstruct->blockSide);
				Odelay = 0;
			}
		}
	}
}