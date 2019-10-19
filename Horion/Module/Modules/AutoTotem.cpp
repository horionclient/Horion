#include "AutoTotem.h"



AutoTotem::AutoTotem() : IModule(0x0, PLAYER, "Automatically puts Totems into your offhand")
{
}


AutoTotem::~AutoTotem()
{
}

const char* AutoTotem::getModuleName()
{
	return ("AutoTotem");
}

void AutoTotem::onTick(C_GameMode* gm) {
	
	if (g_Data.getLocalPlayer() != nullptr) 
	{
		C_ItemStack* offhandTotem = g_Data.getLocalPlayer()->getEquippedTotem();

		if (offhandTotem->item == NULL && delay > 3) 
		{
			C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
			C_Inventory* a = supplies->inventory;
			for (int offhandTotem = 0; offhandTotem < 36; i++) {
				C_ItemStack* test = a->getItemStack(i);
				if (test->item != NULL) {
					C_Item* totem = *test->item;
					if (totem->itemId == 450) {
						g_Data.getLocalPlayer()->consumeTotem();
						g_Data.getLocalPlayer()->setOffhandSlot(test);
					}

				}
			}
			delay = 0;
		}

		delay++;
	}
}


void AutoTotem::onEnable() {
}
