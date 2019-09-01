#pragma once
#include "../Utils/HMath.h"
#include "CInventory.h"
#include "CInventoryTransaction.h"

class C_ComplexInventoryTransaction {
public:
	uintptr_t** vTable; //0x0000
	int actionType;//0x8
	int unknown;//0xC
	C_InventoryTransaction transac;//0x10
};

class C_ItemUseInventoryTransaction : public C_ComplexInventoryTransaction
{
public:
	C_ItemUseInventoryTransaction()
	{
		memset(this,0x0, sizeof(C_ItemUseInventoryTransaction));
		using ItemUseInventoryTransactionContructor = void(__fastcall*)(C_ItemUseInventoryTransaction*);
		static ItemUseInventoryTransactionContructor constructor = reinterpret_cast<ItemUseInventoryTransactionContructor>(Utils::FindSignature("48 89 4C 24 08 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 50 48 89 74 24 58 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 C7 41 ?? ?? ?? ?? ?? 48 8D 59 ?? 48 89 5C 24 ?? 48 8B CB E8 ?? ?? ?? ?? 33 F6 48 89 73 ?? 48 89 73 ?? 48 89 73 ?? 48 8D 05 ?? ?? ?? ?? 48 89 07 48 89 77 ?? 48 89 77 ?? 89 77 ?? 40 88 77 ?? 89"));
		if (constructor != 0)
			constructor(this);
	}
private:
	char pad_0x68[0x4];//0x68
public:
	vec3_ti blockPos;//0x6C
	int runtimeBlockId;//0x78
	int blockSide;//0x7C
	int slot;//0x80
private:
	char pad_0x84[0x4];//0x84
public:
	C_ItemStack item;//0x88
	vec3_t pos;//0x110
	vec3_t clickPos;//0x11C
};
