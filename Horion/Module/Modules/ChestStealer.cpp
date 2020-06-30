#include "ChestStealer.h"

ChestStealer::ChestStealer() : IModule(0x0, Category::PLAYER, "Automatically takes all items out of a chest") {
	registerIntSetting("Close Delay", &this->setDelay, this->setDelay, 0, 20);
}

ChestStealer::~ChestStealer() {
}

const char* ChestStealer::getModuleName() {
	return ("ChestStealer");
}

void ChestStealer::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer()->canOpenContainerScreen() == 0 && chestScreenController != nullptr) {
		std::vector<int> items = {};
		for (int i = 0; i < 54; i++) {
			C_ItemStack* stack = chestScreenController->_getItemStack(TextHolder("container_items"), i);
			if (stack != nullptr && stack->item != NULL)
				items.push_back(i);
		}
		if (!items.empty()) {
			for (int i : items) {
				chestScreenController->handleAutoPlace(0x7FFFFFFF, "container_items", i);
			}
		} else  {
			delay++;
			if (delay > setDelay) {
				chestScreenController->leaveScreen();
				delay = 0;
			}
		}
	}
	chestScreenController = nullptr;
}
