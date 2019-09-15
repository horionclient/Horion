#pragma once
#include "../Utils/HMath.h"
#include "CInventory.h"

class C_InventoryAction
{
public:
	C_InventoryAction() = default;
	C_InventoryAction(int slot, C_ItemStack* sourceItem, C_ItemStack* targetItem, int sourceType = 0,int type = 0)
	{
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
public:
	C_InventoryTransaction()
	{
		memset(this, 0x0, sizeof(C_InventoryTransaction));
		using constructor_t = void(__fastcall*)(C_InventoryTransaction*);
		static constructor_t constructor = reinterpret_cast<constructor_t>(Utils::FindSignature("40 53 48 83 EC 20 48 8B D9 E8 ?? ?? ?? ?? 48 8B 0B BA ?? ?? ?? ?? 48 83 C4 20 5B E9 ?? ?? ?? ?? 40 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 48 48 8B D9 48 89 4C 24 28 33 FF 89 39 48 89 79 08 48 89 79 10 45 33 C0 33 D2 E8 ?? ?? ?? ?? 48 89 43 08 48 89 7B 18 48 89 7B 20 48 89 7B 28 C7 03 ?? ?? ?? ?? 8D 57 08 48 8B CB E8 ?? ?? ?? ?? 90 48 8B C3 48 8B 5C 24 48 48 83 C4 30 5F C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 89 5C 24 08 48 89 74 24 10 57")+32);
		if (constructor != 0)
			constructor(this);
	}
	~C_InventoryTransaction()
	{
		using Destructor_t = void(__fastcall*)(C_InventoryTransaction*);
		static Destructor_t Destructor = reinterpret_cast<Destructor_t>(Utils::FindSignature("40 53 48 83 EC 20 48 8B D9 48 8B 49 18 48 85 C9 74 3F 48 8B 53 28 48 2B D1 48 83 E2 F8 48 ?? ?? ?? ?? ?? ?? 72 18 4C 8B 41 F8 48 83 C2 27 49 2B C8 48 8D 41 F8 48 83 F8 1F 77 32 49 8B C8 E8 ?? ?? ?? ?? 33 C0 48 89 43 18 48 89 43 20 48 89 43 28 48 8D 4B 08 E8 ?? ?? ?? ?? 48 8B 4B 08 BA ?? ?? ?? ?? 48 83 C4 20 5B E9 ?? ?? ?? ?? FF 15 ED 9E 2E 01"));
		if (Destructor != 0)
			Destructor(this);
	}
	void addInventoryAction(C_InventoryAction* action)
	{
		using InventoryTransaction__addAction_t = void(__fastcall*)(C_InventoryTransaction*, C_InventoryAction*);
		static InventoryTransaction__addAction_t InventoryTransaction__addAction = reinterpret_cast<InventoryTransaction__addAction_t>(Utils::FindSignature("4C 8B DC 55 56 57 41 56 41 57 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 49 89 5B 18 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B F2 4C 8B F1 33 FF 49 89 7B B8 4C 8B C2 49 8D 53 B8"));
		if (InventoryTransaction__addAction != 0)
			InventoryTransaction__addAction(this, action);
	}
};

class C_InventoryTransactionManager
{
	C_Player* player;//0x0
	C_InventoryTransaction* transac;//0x8
public:
	void addInventoryAction(C_InventoryAction const& action)
	{
		using InventoryTransactionManager__addAction_t = void(__fastcall*)(C_InventoryTransactionManager*, C_InventoryAction const&);
		static InventoryTransactionManager__addAction_t InventoryTransaction__addAction = reinterpret_cast<InventoryTransactionManager__addAction_t>(Utils::FindSignature("40 55 56 57 41 56 41 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 8B EA 4C 8B F1 4C 8B C2 48 8B 51 ?? 48 8B 49 ?? E8"));
		if (InventoryTransaction__addAction != 0)
			InventoryTransaction__addAction(this, action);
	}
};


