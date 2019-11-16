#include "InventoryCleaner.h"
#include "../ModuleManager.h"

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
	if (g_Data.getLocalPlayer() == nullptr) return;
	if (g_Data.getLocalPlayer()->canOpenContainerScreen() || moduleMgr->getModule<ChestStealer>()->chestScreenController != nullptr) return;

	/*items.clear();
	uselessItems.clear();
	stackableSlot.clear();*/

	/*findStackableItems();   //  ---     causes game crashes
	if (!stackableSlot.empty()) {
		C_InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();
		manager->addInventoryAction(C_InventoryAction(stackableSlot.at(0), g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(stackableSlot.at(0)), nullptr, 632));
	}*/

	std::vector<int> dropSlots = findUselessItems();
	if (!dropSlots.empty()) {
		for (int i : dropSlots) {
			logF("Dropping %d", i);
			g_Data.getLocalPlayer()->getSupplies()->inventory->dropSlot(i); 
		}
		logF("################");
	}
}

void InventoryCleaner::findStackableItems() {
	/*for (int i = 0; i < 36; i++) {
		C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemStack->item != nullptr) {
			if ((*itemStack->item)->getMaxStackSize() > itemStack->count) {
				for (int i2 = 0; i2 < 36; i2++) {
					if (i2 == i) continue;
					C_ItemStack* itemStack2 = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i2);
					if ((*itemStack2->item)->getMaxStackSize() > itemStack->count) {
						if (*itemStack->item == *itemStack2->item) {
							if ((std::find(stackableSlot.begin(), stackableSlot.end(), i2) == stackableSlot.end())) stackableSlot.push_back(i2);
							if ((std::find(stackableSlot.begin(), stackableSlot.end(), i) == stackableSlot.end())) stackableSlot.push_back(i);
						}
					}
				}
			}
		}
	}*/
}

std::vector<int> InventoryCleaner::findUselessItems() {
	// Filter by options


	std::vector<int> uselessItems;
	std::vector<C_Item*> items;

	{
		for (int i = 0; i < 36; i++) {
			C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
			if (itemStack->item != nullptr) {
				if (!stackIsUseful(itemStack)) {
					if (std::find(items.begin(), items.end(), *itemStack->item) == items.end())
						uselessItems.push_back(i);
					else
						items.push_back(*itemStack->item);
				}else if (std::find(items.begin(), items.end(), *itemStack->item) == items.end())
					items.push_back(*itemStack->item);
			}
		}
		/*
		for (int i = 0; i < 4; i++) {
			if (g_Data.getLocalPlayer()->getArmor(i)->item != nullptr)
				items.push_back(reinterpret_cast<C_Item*>(*g_Data.getLocalPlayer()->getArmor(i)->item));
		}
		*/
	}
	// Filter weapons
	
	{
		// Filter by attack damage
		std::sort(items.begin(), items.end(), [](const C_Item* lhs, const C_Item* rhs)
			{
				C_Item* current = const_cast<C_Item*>(lhs);
				C_Item* other = const_cast<C_Item*>(rhs);
				return current->getAttackDamage() > other->getAttackDamage();
			});

		bool hadTheBestItem = false;
		C_Item* bestItem = items.at(0);
		for (int i = 0; i < 36; i++) {
			if (std::find(uselessItems.begin(), uselessItems.end(), i) != uselessItems.end())
				continue;
			C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
			
			if (itemStack->item != nullptr) {
				if ((*itemStack->item)->getAttackDamage() < bestItem->getAttackDamage()) {
					uselessItems.push_back(i);
				}
				else {
					// Damage same as bestItem
					if (hadTheBestItem)
						uselessItems.push_back(i);
					else
						hadTheBestItem = true;
				}
			}
		}
	}
	
	// Filter armor
	{
		std::vector<C_Item*> helmets;
		std::vector<C_Item*> chestplates;
		std::vector<C_Item*> leggins;
		std::vector<C_Item*> boots;

		// Filter by armor value
		std::sort(items.begin(), items.end(), [](const C_Item* lhs, const C_Item* rhs)
		{
			C_Item* current = const_cast<C_Item*>(lhs);
			C_Item* other = const_cast<C_Item*>(rhs);
			return current->getArmorValue() > other->getArmorValue();
		});
		
		// Put armor items in their respective vectors
		for (C_Item* item : items)
		{
			if (item->isHelmet()) helmets.push_back(item);
			else if (item->isChestplate()) chestplates.push_back(item);
			else if (item->isLeggins()) leggins.push_back(item);
			else if (item->isBoots()) boots.push_back(item);
		}

		bool hadBest[4];
		for (int i = 0; i < 36; i++) {
			if (std::find(uselessItems.begin(), uselessItems.end(), i) != uselessItems.end())
				continue;
			C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
			if (itemStack->item != nullptr) {
				if ((*itemStack->item)->isHelmet()) {
					if (hadBest[0] || (*itemStack->item) != helmets.at(0)) {
						logF("%llX != %llX  %d %d", (*itemStack->item), helmets.at(0), (*itemStack->item)->getArmorValue(), helmets.at(0)->getArmorValue());
						uselessItems.push_back(i);
					}
						
					else 
						hadBest[0] = true;
				}
				else if ((*itemStack->item)->isChestplate()) {
					if (hadBest[1] || (*itemStack->item) != chestplates.at(0))
						uselessItems.push_back(i);
					else
						hadBest[1] = true;
				}
				else if ((*itemStack->item)->isLeggins()) {
					if (hadBest[2] || (*itemStack->item) != leggins.at(0))
						uselessItems.push_back(i);
					else
						hadBest[2] = true;
				}
				else if ((*itemStack->item)->isBoots()) {
					if (hadBest[3] || (*itemStack->item) != boots.at(0))
						uselessItems.push_back(i);
					else
						hadBest[3] = true;
				}
			}
		}
	}

	return uselessItems;
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