#include "ChestStealer.h"



ChestStealer::ChestStealer() : IModule(0x0, Category::PLAYER, "Automatically takes all items out of a chest") {
	registerIntSetting("Close Delay", &this->setDelay, this->setDelay, 3, 20);
}


ChestStealer::~ChestStealer() {

}

const char* ChestStealer::getModuleName() {
	return ("ChestStealer");
}

void ChestStealer::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer()->canOpenContainerScreen() == 0 && chestScreenController != nullptr) { 
		for (int i = 0; i < 54; i++) {
			chestScreenController->handleAutoPlace(0x7FFFFFFF,"container_items", i);
		}
		delay++;
	}
	if (g_Data.getLocalPlayer()->canOpenContainerScreen() == 0 && chestScreenController != nullptr && delay > setDelay) {
		chestScreenController->leaveScreen();
		delay = 0;
	}
		
	chestScreenController = nullptr;
}


