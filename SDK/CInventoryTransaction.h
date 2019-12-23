#pragma once
#include "../Utils/HMath.h"
#include "CInventory.h"

class C_InventoryAction {
public:
	C_InventoryAction() = default;
	C_InventoryAction(int slot, C_ItemStack* sourceItem, C_ItemStack* targetItem, int sourceType = 0,int type = 0) {
		memset(this, 0x0, sizeof(C_InventoryAction));
		this->slot = slot;
		if(sourceItem != nullptr)
			this->sourceItem = *sourceItem;
		if(targetItem != nullptr)
			this->targetItem = *targetItem;
		this->sourceType = sourceType;
		this->type = type;
	}
public:
	int type;//0x0
	int sourceType;//0x4
private:
	int unknown;//0x8
public:
	int slot;//0xC
	C_ItemStack sourceItem;//0x10
	C_ItemStack targetItem;//0x98
};

class C_InventoryTransaction {
private:
	char pad_0x0[0x58];//0x0
};

class C_InventoryTransactionManager {
public:
	uintptr_t* player; //0x0
	C_InventoryTransaction transac; //0x8
private:
	int unknown; //0x60
	// Total size: 0x68
public:
	void addInventoryAction(C_InventoryAction const& action) {
		using InventoryTransactionManager__addAction_t = void(__fastcall*)(C_InventoryTransactionManager*, C_InventoryAction const&);
		static InventoryTransactionManager__addAction_t InventoryTransactionManager__addAction = reinterpret_cast<InventoryTransactionManager__addAction_t>(Utils::FindSignature("40 55 56 57 41 56 41 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 8B EA 4C 8B F1 4C 8B C2 48 8B 51 ?? 48 8B 49 ?? E8"));
		if (InventoryTransactionManager__addAction != 0)
			InventoryTransactionManager__addAction(this, action);
	}
};


