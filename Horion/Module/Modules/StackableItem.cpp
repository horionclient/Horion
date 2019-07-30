#include "StackableItem.h"



StackableItem::StackableItem() : IModule(0x0, PLAYER)
{
}


StackableItem::~StackableItem()
{
}

const char* StackableItem::getModuleName()
{
	return ("StackableItem");
}

void StackableItem::onTick(C_GameMode* gm) {

	if (g_Data.getLocalPlayer() != nullptr)
	{
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* a = supplies->inventory;
		for (int i = 0; i < 36; i++) {
			C_ItemStack* test = a->getItemStack(i);
			if (test->item != NULL) {
				C_Item* yikes = *test->item;
				yikes->setMaxStackSize(64);
			}
		}
	}
}


void StackableItem::onEnable() {
}