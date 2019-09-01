#pragma once
#include "../Utils/HMath.h"
#include "CInventory.h"

class C_InventoryTransaction {
	char pad_0x0[0x58];//0x0
};

class C_InventoryTransactionManager
{
	C_Player* player;//0x0
	C_InventoryTransaction* transac;//0x8
};

class C_InventoryAction
{
private:
	char pad_0x0[0x8];//0x0
public:
	int sourceType;//0x8
	int slot;//0xC
	C_ItemStack sourceItem;//0x10
	C_ItemStack targetItem;//0x98
};

