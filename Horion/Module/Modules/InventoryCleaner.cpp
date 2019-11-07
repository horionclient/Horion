#include "InventoryCleaner.h"

InventoryCleaner::InventoryCleaner() : IModule(0x0, Category::PLAYER, "Automatically throws not needed stuff out of your inventory")
{
	registerBoolSetting("Tools", &this->keepTools, this->keepTools);
	registerBoolSetting("Armor", &this->keepArmor, this->keepArmor);
	registerBoolSetting("Food", &this->keepFood, this->keepFood);
	registerBoolSetting("Blocks", &this->keepBlocks, this->keepBlocks);
}


InventoryCleaner::~InventoryCleaner()
{
}

const char* InventoryCleaner::getModuleName()
{
	return ("InventoryCleaner");
}

void InventoryCleaner::onTick(C_GameMode* gm)
{
	if (gm->player == nullptr) return;

	uselessItems.clear();
	stackableSlot.clear();

	//findStackableItems();     ---     causes game crashes
	if (!stackableSlot.empty()) {
		C_InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();
		manager->addInventoryAction(C_InventoryAction(stackableSlot.at(0), g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(stackableSlot.at(0)), nullptr, 632));
		delete manager;
	}

	findUselessItems();
	if (!uselessItems.empty()) {
		for (int i : uselessItems) {
			for(int n = 0; n < g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i)->count; n++)
			g_Data.getLocalPlayer()->getSupplies()->inventory->dropSlot(i); // this code is stupid, please change to drop full stack
		}
	}
}

void InventoryCleaner::findStackableItems() {
	for (int i = 0; i < 40; i++) {
		C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemStack->item != nullptr) {
			if ((*itemStack->item)->getMaxStackSize() > itemStack->count) {
				for (int i2 = 0; i2 < 36; i2++) {
					if (i2 == i) continue;
					C_ItemStack* itemStack2 = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i2);
					if ((*itemStack2->item)->getMaxStackSize() > itemStack->count) {
						if (*itemStack->item == *itemStack2->item) {
							if (!(std::find(stackableSlot.begin(), stackableSlot.end(), i2) != stackableSlot.end())) stackableSlot.push_back(i2);
							if (!(std::find(stackableSlot.begin(), stackableSlot.end(), i) != stackableSlot.end())) stackableSlot.push_back(i);
						}
					}
				}
			}
		}
	}
}

void InventoryCleaner::findUselessItems() {
	items.clear();
	for (int i = 0; i < 36; i++) {
		C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemStack->item != nullptr) {
			if (!stackIsUseful(itemStack)) {
				if (!(std::find(items.begin(), items.end(), (*itemStack->item)) != items.end())) uselessItems.push_back(i);
			}
			items.push_back((*itemStack->item));
			for (C_Item* item : items) {
				if ((*itemStack->item)->isTool() && (*itemStack->item)->getAttackDamage() <= item->getAttackDamage())
					if(!isLastItem(*itemStack->item)) uselessItems.push_back(i);
				if ((*itemStack->item)->isArmor() && (*itemStack->item)->getArmorValue() <= item->getArmorValue() && !isLastItem((*itemStack->item)))
					if (!isLastItem(*itemStack->item)) uselessItems.push_back(i);
			}
		}
	}
}

bool InventoryCleaner::stackIsUseful(C_ItemStack* itemStack) {
	if (itemStack->item == nullptr) return true;
	if (keepArmor && (*itemStack->item)->isArmor()) return true; // Armor
	if (keepTools && (*itemStack->item)->isTool()) return true; // Tools
	if (keepFood && (*itemStack->item)->isFood()) return true; // Food
	if (keepBlocks && (*itemStack->item)->isBlock()) return true; // Block
	if (keepTools && (*itemStack->item)->itemId == 368) return true; // Ender Pearl
	return false;
}

bool InventoryCleaner::isLastItem(C_Item* item) {
	std::vector<C_Item*> items;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (stack->item != nullptr) items.push_back((*stack->item));
	}
	int count = 0;
	for (C_Item* a : items) {
		if (a == item) count++;
	}
	if (count > 1) return false;
	return true;
}