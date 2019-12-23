#pragma once
#include "../Utils/HMath.h"
#include "CInventory.h"
#include "CInventoryTransaction.h"

class C_ComplexInventoryTransaction {
public:
	C_ComplexInventoryTransaction() {
		memset(this, 0, sizeof(C_ComplexInventoryTransaction));
	}
	C_ComplexInventoryTransaction(C_InventoryTransaction& transac) {
		static uintptr_t** ComplexInventoryTransactionVtable = 0x0;
		if (ComplexInventoryTransactionVtable == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("48 8D 15 ?? ?? ?? ?? 49 89 53 C0 49 89 43 E0 48 8B 01 49 8D 53 C0");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			ComplexInventoryTransactionVtable = reinterpret_cast<uintptr_t * *>(sigOffset + offset + /*length of instruction*/ 7);
			if (ComplexInventoryTransactionVtable == 0x0 || sigOffset == 0x0)
				logF("C_ComplexInventoryTransactionVtable signature not working!!!");
		}
		memset(this, 0, sizeof(C_ComplexInventoryTransaction)); // Avoid overwriting vtable
		vTable = ComplexInventoryTransactionVtable;
		uintptr_t reinterpretInvTransaction = reinterpret_cast<uintptr_t>(this);
		using constructor_t = void(__fastcall*)(uintptr_t,C_InventoryTransaction&);
		static constructor_t constructor = reinterpret_cast<constructor_t>(Utils::FindSignature("48 89 4C 24 ?? 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 89 74 24 ?? 48 8B DA 48 8B F9 48 89 4C 24 ?? 8B 02 89 01 33 F6 48 89 71 ??"));
		if (constructor != 0)
			constructor(reinterpretInvTransaction+0x10,transac);
		this->actionType = 0;
	}
	uintptr_t** vTable; //0x0000
	int actionType;//0x8
	int unknown;//0xC
	C_InventoryTransaction transac;//0x10
};

class C_ItemUseInventoryTransaction : public C_ComplexInventoryTransaction {
public:
	C_ItemUseInventoryTransaction() {
		memset(this,0x0, sizeof(C_ItemUseInventoryTransaction));
		using ItemUseInventoryTransactionContructor = void(__fastcall*)(C_ItemUseInventoryTransaction*);
		static ItemUseInventoryTransactionContructor constructor = reinterpret_cast<ItemUseInventoryTransactionContructor>(Utils::FindSignature("48 89 4C 24 08 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 50 48 89 74 24 58 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 C7 41 ?? ?? ?? ?? ?? 48 8D 59 ?? 48 89 5C 24 ?? 48 8B CB E8 ?? ?? ?? ?? 33 F6 48 89 73 ?? 48 89 73 ?? 48 89 73 ?? 48 8D 05 ?? ?? ?? ?? 48 89 07 48 89 77 ?? 48 89 77 ?? 89 77 ?? 40 88 77 ?? 89"));
		if (constructor != 0)
			constructor(this);
	}
	C_ItemUseInventoryTransaction(int slot,C_ItemStack const* item,vec3_t const pos,int blockSide = 255,int runtimeBlockId = 0) {
		memset(this, 0x0, sizeof(C_ItemUseInventoryTransaction));
		using ItemUseInventoryTransactionContructor = void(__fastcall*)(C_ItemUseInventoryTransaction*);
		static ItemUseInventoryTransactionContructor constructor = reinterpret_cast<ItemUseInventoryTransactionContructor>(Utils::FindSignature("48 89 4C 24 08 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 50 48 89 74 24 58 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 C7 41 ?? ?? ?? ?? ?? 48 8D 59 ?? 48 89 5C 24 ?? 48 8B CB E8 ?? ?? ?? ?? 33 F6 48 89 73 ?? 48 89 73 ?? 48 89 73 ?? 48 8D 05 ?? ?? ?? ?? 48 89 07 48 89 77 ?? 48 89 77 ?? 89 77 ?? 40 88 77 ?? 89"));
		if (constructor != 0)
			constructor(this);
		this->slot = slot;
		this->item = *item;
		this->pos = pos;
		this->runtimeBlockId = runtimeBlockId;
		this->blockSide = blockSide;
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

class C_ItemReleaseInventoryTransaction : public C_ComplexInventoryTransaction {
public:
	C_ItemReleaseInventoryTransaction() {
		memset(this, 0x0, sizeof(C_ItemReleaseInventoryTransaction));
		using ItemReleaseInventoryTransactionContructor = void(__fastcall*)(C_ItemReleaseInventoryTransaction*);
		static ItemReleaseInventoryTransactionContructor constructor = reinterpret_cast<ItemReleaseInventoryTransactionContructor>(Utils::FindSignature("48 89 4C 24 ?? 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 89 74 24 ?? 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 C7 41 ?? ?? ?? ?? ?? 48 8D 59 ?? 48 89 5C 24 ?? 48 8B CB"));
		if (constructor != 0)
			constructor(this);
	}
	C_ItemReleaseInventoryTransaction(int slot,C_ItemStack const* item ,vec3_t const pos) {
		memset(this, 0x0, sizeof(C_ItemReleaseInventoryTransaction));
		using ItemReleaseInventoryTransactionContructor = void(__fastcall*)(C_ItemReleaseInventoryTransaction*);
		static ItemReleaseInventoryTransactionContructor constructor = reinterpret_cast<ItemReleaseInventoryTransactionContructor>(Utils::FindSignature("48 89 4C 24 ?? 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 89 74 24 ?? 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 C7 41 ?? ?? ?? ?? ?? 48 8D 59 ?? 48 89 5C 24 ?? 48 8B CB"));
		if (constructor != 0)
			constructor(this);
		this->slot = slot;
		this->item = *item;
		this->pos = pos;
	}
private:
	char pad_0x68[0x4];//0x68
public:
	int slot;//0x6C
	C_ItemStack item;//0x70
	vec3_t pos;//0xF8
	int unknown;//0x104
};
