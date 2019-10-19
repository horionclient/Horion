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

        if (offHandTotem->item == NULL && delay > 3) 
        {
            C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
            C_Inventory* inventory = supplies->inventory;
            for (int slot = 0; slot < 36; slot++) {
                C_ItemStack* slotItem = slot->getItemStack(i);
                if (slotItem->item != NULL) {
                    C_Item* slotItemId = *slotItem->item;
                    if (slotItemId->item == 450) {
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
