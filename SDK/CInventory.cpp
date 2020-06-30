#include "CInventory.h"
#include "../Utils/Utils.h"
#include "../Memory/GameData.h"
void C_Inventory::dropSlot(int slot) {
	// FillingContainer::dropSlot
	using drop_t = void(__fastcall*)(C_Inventory*, int, char);
	static drop_t func = reinterpret_cast<drop_t>(FindSignature("85 D2 0F 88 ?? ?? ?? ?? 55 56 57 41 54 41 55 41 56 41 57 48"));
	if (func != 0)
		func(this, slot, 0);
}
void C_Inventory::dropAll(int slot) {
	// FillingContainer::dropAll
	using dropAll_t = void(__fastcall*)(C_Inventory*, int, int, char);
	static dropAll_t func = reinterpret_cast<dropAll_t>(FindSignature("48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 48 89 7C 24 ?? 41 56 48 83 EC ?? 48 8B 01 41 0F"));
	if (func != 0)
		func(this, slot, 0, 0);
}
bool C_Inventory::isFull() {
	int fullslots = 0;
	for (int i = 0; i < 36; i++) {
		if (this->getItemStack(i)->item != nullptr)
			fullslots++;
	}
	if (fullslots == 36) return true;
	return false;
}
int C_Inventory::getFirstEmptySlot() {
	for (int i = 0; i < 36; i++) {
		if (this->getItemStack(i)->item == nullptr)
			return i;
	}
	return -1;
}
void C_ContainerScreenController::handleAutoPlace(uintptr_t a1, std::string name, int slot) {
	using ContainerScreenController__autoPlace = __int64(__fastcall*)(C_ContainerScreenController*, uintptr_t, TextHolder, int);
	static ContainerScreenController__autoPlace autoPlaceFunc = reinterpret_cast<ContainerScreenController__autoPlace>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 C7 44 24 ? ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 45 8B E1 49 8B F8"));

	TextHolder txt = TextHolder(name);

	if (autoPlaceFunc != 0x0)
		autoPlaceFunc(this, a1, txt, slot);
}

void C_Inventory::moveItem(int from, int to = -1) {
	C_InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();

	uintptr_t sigOffset = FindSignature("48 8D 3D ? ? ? ? 80 B8 ? ? ? ? ? 75 19 48 8B 88 ? ? ? ? 48 8B 11 4C 8B 42 28 8B 50 10");
	int offset = *reinterpret_cast<int*>(sigOffset + 3);
	C_ItemStack* emptyItemStack = reinterpret_cast<C_ItemStack*>(sigOffset + offset + /*length of instruction*/ 7);

	if (to < 0) to = getFirstEmptySlot();
	C_ItemStack* item1 = getItemStack(from);
	C_ItemStack* item2 = getItemStack(to);

	if (item1->item == NULL) return;
	if (item2->item == NULL) {
		C_InventoryAction first(from, item1, nullptr);
		C_InventoryAction second(to, nullptr, item1);
		manager->addInventoryAction(first);
		manager->addInventoryAction(second);
		*item2 = *item1;
		*item1 = *emptyItemStack;
	} else {
		C_InventoryAction first(from, item1, nullptr);
		C_InventoryAction second(to, item2, item1);
		C_InventoryAction third(from, nullptr, item2);
		manager->addInventoryAction(first);
		manager->addInventoryAction(second);
		manager->addInventoryAction(third);
		C_ItemStack a = *item2;
		*item2 = *item1;
		*item1 = a;
	}
}
