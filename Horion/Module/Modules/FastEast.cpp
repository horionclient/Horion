#include "FastEat.h"



FastEat::FastEat() : IModule(0x0, PLAYER)
{
}


FastEat::~FastEat()
{
}

const char* FastEat::getModuleName()
{
	return ("FastEat");
}

void FastEat::onTick(C_GameMode* gm)
{
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = inv->getItemStack(i);
		if (stack->item != NULL && (*stack->item)->itemId != 261 && (*stack->item)->duration == 32)
		{
			(*stack->item)->setMaxUseDuration(5);
		}
	}
}