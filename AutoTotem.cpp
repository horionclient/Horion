#include "AutoTotem.h"

AutoTotem::AutoTotem() : IModule(0x0, PLAYER)
{
	this->registerBoolSetting("Create Totems", &this->isSOH, this->isSOH);
	this->registerIntSetting("Delay (Ticks)", &this->tickDelay, this->tickDelay, 1, 20);
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
		C_ItemStack* i = g_Data.getLocalPlayer()->getEquippedTotem();
		if (delay > tickDelay) 
		{
			if (isSOH == true)
			{
				int itemId = 450;
				char count = 1;
				char itemData = 0;

				using getItemFromId_t = C_ItemStack * (__fastcall*)(void*, int itemID);
				static getItemFromId_t  getItemFromId = reinterpret_cast<getItemFromId_t>(Utils::FindSignature("40 53 48 83 EC 30 48 8B D9 66 85 D2 0F 84 ?? ?? ?? ?? 44 0F BF CA 48 8D 4C 24 ?? 44 89 4C 24 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 66 0F 1F 44 00 00 0F B6 01 48 8D 49 ??"));

				void* ItemPtr = malloc(0x8);
				C_ItemStack* cStack = getItemFromId(ItemPtr, itemId);
				C_ItemStack* sohItem = reinterpret_cast<C_ItemStack*>(malloc(0x88));
				sohItem->ItemStackConstructor(*cStack->item, count, itemData);
				g_Data.getLocalPlayer()->setOffhandSlot(sohItem);
				delay = 0;
			}
			else if (isSOH == false)
			{
				C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
				C_Inventory* slot = supplies->inventory; //slot = player inventory

				for (int i = 0; i < 36; i++) { //loop through inventory slots
					C_ItemStack* currentSlot = slot->getItemStack(i); //get currentSlot

					if (currentSlot->item != NULL) { //is currentSlot is not empty?
						C_Item* maybeTotem = *currentSlot->item; //currentSlot may be totem

						if (maybeTotem->itemId == 450) { //is currentSlot totem?
							C_ItemStack* totem = slot->getItemStack(i); //Slot i is totem, set as such
							g_Data.getLocalPlayer()->consumeTotem(); //Remove Totem in Slot?
							g_Data.getLocalPlayer()->setOffhandSlot(totem); //Add Totem to Offhand
						}

					}
				}
				delay = 0;
			}
		}
		else if (i->item == NULL) {
			delay++;
		}
	}
}


void AutoTotem::onEnable() 
{
	if (g_Data.getLocalPlayer() == nullptr)
		this->setEnabled(false);
}