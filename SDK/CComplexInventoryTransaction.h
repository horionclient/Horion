#pragma once
#include "../Utils/HMath.h"
#include "CInventory.h"
#include "CInventoryTransaction.h"

class C_ComplexInventoryTransaction {
public:
	C_ComplexInventoryTransaction();
	C_ComplexInventoryTransaction(C_InventoryTransaction& transac);
	uintptr_t** vTable;              //0x0000
	int actionType;                  //0x8
	int unknown;                     //0xC
	C_InventoryTransaction transac;  //0x10
};

class C_ItemUseInventoryTransaction : public C_ComplexInventoryTransaction {
public:
	C_ItemUseInventoryTransaction();
	C_ItemUseInventoryTransaction(int slot, C_ItemStack const* item, vec3_t const pos, int blockSide = 255, int runtimeBlockId = 0);

private:
	char pad_0x68[0x4];  //0x68
public:
	vec3_ti blockPos;    //0x6C
	int runtimeBlockId;  //0x78
	int blockSide;       //0x7C
	int slot;            //0x80
private:
	char pad_0x84[0x4];  //0x84
public:
	C_ItemStack item;  //0x88
	vec3_t pos;        //0x110
	vec3_t clickPos;   //0x11C
};

class C_ItemReleaseInventoryTransaction : public C_ComplexInventoryTransaction {
public:
	C_ItemReleaseInventoryTransaction();
	C_ItemReleaseInventoryTransaction(int slot, C_ItemStack const* item, vec3_t const pos);

private:
	char pad_0x68[0x4];  //0x68
public:
	int slot;          //0x6C
	C_ItemStack item;  //0x70
	vec3_t pos;        //0xF8
	int unknown;       //0x104
};
