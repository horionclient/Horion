#include "CComplexInventoryTransaction.h"
#include "../Utils/Utils.h"
C_ComplexInventoryTransaction::C_ComplexInventoryTransaction(C_InventoryTransaction& transac) {
	static uintptr_t** ComplexInventoryTransactionVtable = 0x0;
	if (ComplexInventoryTransactionVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 03 89 73 08 48");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		ComplexInventoryTransactionVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (ComplexInventoryTransactionVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_ComplexInventoryTransaction));  // Avoid overwriting vtable
	vTable = ComplexInventoryTransactionVtable;
	uintptr_t boi = reinterpret_cast<uintptr_t>(this);
	using constructor_t = void(__fastcall*)(uintptr_t, C_InventoryTransaction&);
	static constructor_t constructor = reinterpret_cast<constructor_t>(FindSignature("48 89 4C 24 ?? 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 89 74 24 ?? 48 8B DA 48 8B F9 48 89 4C 24 ?? 8B 02 89 01 33 F6 48 89 71 ??"));
	if (constructor != 0)
		constructor(boi + 0x10, transac);
	this->actionType = 0;
}
C_ComplexInventoryTransaction::C_ComplexInventoryTransaction() {
	memset(this, 0, sizeof(C_ComplexInventoryTransaction));
	static uintptr_t** ComplexInventoryTransactionVtable = 0x0;
	if (ComplexInventoryTransactionVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 03 89 73 08 48");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		ComplexInventoryTransactionVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (ComplexInventoryTransactionVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	vTable = ComplexInventoryTransactionVtable;
}
C_ItemUseInventoryTransaction::C_ItemUseInventoryTransaction() {
	memset(this, 0x0, sizeof(C_ItemUseInventoryTransaction));
	using ItemUseInventoryTransactionContructor = void(__fastcall*)(C_ItemUseInventoryTransaction*);
	static ItemUseInventoryTransactionContructor constructor = reinterpret_cast<ItemUseInventoryTransactionContructor>(FindSignature("48 89 4C 24 08 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 50 48 89 74 24 58 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 C7 41 ?? ?? ?? ?? ?? 48 8D 59 ?? 48 89 5C 24 ?? 48 8B CB E8 ?? ?? ?? ?? 33 F6 48 89 73 ?? 48 89 73 ?? 48 89 73 ?? 48 8D 05 ?? ?? ?? ?? 48 89 07 48 89 77 ?? 48 89 77 ?? 89 77 ?? 40 88 77 ?? 89"));
	if (constructor != 0)
		constructor(this);
}
C_ItemUseInventoryTransaction::C_ItemUseInventoryTransaction(int slot, const C_ItemStack* item, const vec3_t pos, int blockSide, int runtimeBlockId) {
	memset(this, 0x0, sizeof(C_ItemUseInventoryTransaction));
	using ItemUseInventoryTransactionContructor = void(__fastcall*)(C_ItemUseInventoryTransaction*);
	static ItemUseInventoryTransactionContructor constructor = reinterpret_cast<ItemUseInventoryTransactionContructor>(FindSignature("48 89 4C 24 08 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 50 48 89 74 24 58 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 C7 41 ?? ?? ?? ?? ?? 48 8D 59 ?? 48 89 5C 24 ?? 48 8B CB E8 ?? ?? ?? ?? 33 F6 48 89 73 ?? 48 89 73 ?? 48 89 73 ?? 48 8D 05 ?? ?? ?? ?? 48 89 07 48 89 77 ?? 48 89 77 ?? 89 77 ?? 40 88 77 ?? 89"));
	if (constructor != 0)
		constructor(this);
	this->slot = slot;
	this->item = *item;
	this->pos = pos;
	this->runtimeBlockId = runtimeBlockId;
	this->blockSide = blockSide;
}
C_ItemReleaseInventoryTransaction::C_ItemReleaseInventoryTransaction() {
	memset(this, 0x0, sizeof(C_ItemReleaseInventoryTransaction));
	using ItemReleaseInventoryTransactionContructor = void(__fastcall*)(C_ItemReleaseInventoryTransaction*);
	static ItemReleaseInventoryTransactionContructor constructor = reinterpret_cast<ItemReleaseInventoryTransactionContructor>(FindSignature("48 89 4C 24 ?? 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 89 74 24 ?? 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 C7 41 ?? ?? ?? ?? ?? 48 8D 59 ?? 48 89 5C 24 ?? 48 8B CB"));
	if (constructor != 0)
		constructor(this);
}
C_ItemReleaseInventoryTransaction::C_ItemReleaseInventoryTransaction(int slot, const C_ItemStack* item, const vec3_t pos) {
	memset(this, 0x0, sizeof(C_ItemReleaseInventoryTransaction));
	using ItemReleaseInventoryTransactionContructor = void(__fastcall*)(C_ItemReleaseInventoryTransaction*);
	static ItemReleaseInventoryTransactionContructor constructor = reinterpret_cast<ItemReleaseInventoryTransactionContructor>(FindSignature("48 89 4C 24 ?? 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 89 74 24 ?? 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 C7 41 ?? ?? ?? ?? ?? 48 8D 59 ?? 48 89 5C 24 ?? 48 8B CB"));
	if (constructor != 0)
		constructor(this);
	this->slot = slot;
	this->item = *item;
	this->pos = pos;
}
