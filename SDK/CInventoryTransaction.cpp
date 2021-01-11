#include "CInventoryTransaction.h"
#include "../Utils/Utils.h"
void C_InventoryTransactionManager::addInventoryAction(const C_InventoryAction& action,bool idk) {
	using InventoryTransactionManager__addAction_t = void(__fastcall*)(C_InventoryTransactionManager*, C_InventoryAction const&,bool);
	//static auto InventoryTransactionManager__addAction = reinterpret_cast<InventoryTransactionManager__addAction_t>(FindSignature("40 55 56 57 41 56 41 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 8B EA 4C 8B F1 4C 8B C2 48 8B 51 ?? 48 8B 49 ?? E8"));
	static auto InventoryTransactionManager__addAction = reinterpret_cast<InventoryTransactionManager__addAction_t>(FindSignature("48 89 5C 24 ? 55 56 57 41 56 41 57 48 83 EC ? 45 0F B6 F8 48 8B EA 4C 8B F1 48 8B 01 48 8B 88 ? ? ? ? 48 85 C9"));
	if (InventoryTransactionManager__addAction != 0)
		InventoryTransactionManager__addAction(this, action,idk);
}
