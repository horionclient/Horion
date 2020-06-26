#include "CInventory.h"
#include "../Utils/Utils.h"
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
	static ContainerScreenController__autoPlace autoPlaceFunc = reinterpret_cast<ContainerScreenController__autoPlace>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 C7 44 24 ? ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 45 8B E1 49 8B F8 44 8B EA 89 54 24 ? 48 8B F1 49 BF ? ? ? ? ? ? ? ?"));

	TextHolder txt = TextHolder(name);

	if (autoPlaceFunc != 0x0)
		autoPlaceFunc(this, a1, txt, slot);
}
