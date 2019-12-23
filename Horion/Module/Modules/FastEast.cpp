#include "FastEat.h"



FastEat::FastEat() : IModule(0x0, Category::PLAYER, "Eat food almost instantly!") {
}


FastEat::~FastEat() {
}

const char* FastEat::getModuleName() {
	return ("FastEat");
}

void FastEat::onTick(C_GameMode* gm) {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = inv->getItemStack(i);
		if (stack->item != NULL && (*stack->item)->itemId != 261 && (*stack->item)->duration == 32) {
			(*stack->item)->setMaxUseDuration(5);
		}
	}
}

void FastEat::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = inv->getItemStack(i);
		if (stack->item != NULL && (*stack->item)->itemId != 261 && (*stack->item)->duration == 5) {
			(*stack->item)->setMaxUseDuration(32);
		}
	}
}