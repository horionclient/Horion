#pragma once
#include "../Utils/HMath.h"
#include "CItem.h"
#include "CInventory.h"

class C_InventoryAction {
public:
	C_InventoryAction() = default;
	C_InventoryAction(int slot, C_ItemStack* sourceItem, C_ItemStack* targetItem, int sourceType = 0, int type = 0) {
		memset(this, 0x0, sizeof(C_InventoryAction));
		this->slot = slot;
		if (sourceItem != nullptr)
			this->sourceItem = *sourceItem;
		if (targetItem != nullptr)
			this->targetItem = *targetItem;
		this->sourceType = sourceType;
		this->type = type;
	}

public:
	int type;        //0x0  // named sourceType in nukkit
	int sourceType;  //0x4 // sometimes windowId
private:
	int unknown;  //0x8
public:
	int slot;                //0xC
	C_ItemStack sourceItem;  //0x10
	C_ItemStack targetItem;  //0x98
};

class C_InventoryTransaction {
private:
	char pad_0x0[8];
public:
	__int64 ptr; // 0x008
private:
	char pad_0x10[0x58 - 16];  //0x10
};

class C_InventoryTransactionManager {
public:
	uintptr_t* player;               //0x0
	C_InventoryTransaction transac;  //0x8
private:
	int unknown;  //0x60
				  // Total size: 0x68
public:
	void addInventoryAction(C_InventoryAction const& action , bool idk = false);
};
