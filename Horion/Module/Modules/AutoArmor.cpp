#include "AutoArmor.h"
#include "../../../Utils/Utils.h"
#include "../../../Utils/Logger.h"

class ArmorStruct {
public:
	ArmorStruct(C_ItemStack* item, C_ArmorItem* yot, int slot) {
		armor = yot;
		m_slot = slot;
		m_item = item;
	}
	bool isEqual(ArmorStruct& src) {
		if (this->m_item->getArmorValueWithEnchants() == src.m_item->getArmorValueWithEnchants())
			return true;
		else
			return false;
	}

	bool operator()(ArmorStruct lhs, ArmorStruct rhs) {
		return (lhs.m_item->getArmorValueWithEnchants() > rhs.m_item->getArmorValueWithEnchants());
	}
	C_ArmorItem* armor = nullptr;
	C_ItemStack* m_item = nullptr;
	int m_slot = 0;
};

AutoArmor::AutoArmor() : IModule(0x0, Category::PLAYER, "Automatically equips the best armor") {
}

AutoArmor::~AutoArmor() {
}

const char* AutoArmor::getModuleName() {
	return ("AutoArmor");
}

void AutoArmor::onTick(C_GameMode* gm) {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();

	C_InventoryAction* first = nullptr;
	C_InventoryAction* second = nullptr;
	static C_ItemStack* emptyItemStack = nullptr;

	if (emptyItemStack == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 3D ? ? ? ? 80 B8 ? ? ? ? ? 75 19 48 8B 88 ? ? ? ? 48 8B 11 4C 8B 42 28 8B 50 10");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		emptyItemStack = reinterpret_cast<C_ItemStack*>(sigOffset + offset + /*length of instruction*/ 7);
	}

	std::vector<ArmorStruct> armorList;

	struct CompareArmorStruct {
		bool operator()(ArmorStruct lhs, ArmorStruct rhs) {
			return (lhs.m_item->getArmorValueWithEnchants() > rhs.m_item->getArmorValueWithEnchants());
		}
	};

	for (int i = 0; i < 4; i++) {
		for (int n = 0; n < 36; n++) {
			C_ItemStack* stack = inv->getItemStack(n);
			if (stack->item != NULL && (*stack->item)->isArmor() && reinterpret_cast<C_ArmorItem*>(*stack->item)->ArmorSlot == i) {
				armorList.push_back(ArmorStruct(stack, reinterpret_cast<C_ArmorItem*>(*stack->item), n));
			}
		}

		if (gm->player->getArmor(i)->item != nullptr)
			armorList.push_back(ArmorStruct(gm->player->getArmor(i), reinterpret_cast<C_ArmorItem*>(*gm->player->getArmor(i)->item), i));

		if (armorList.size() > 0) {
			std::sort(armorList.begin(), armorList.end(), CompareArmorStruct());
			C_ItemStack* armorItem = gm->player->getArmor(i);

			if (armorItem->item != nullptr && (ArmorStruct(armorItem, reinterpret_cast<C_ArmorItem*>(*armorItem->item), 0).isEqual(armorList[0])) == false) {
				int slot = inv->getFirstEmptySlot();

				first = new C_InventoryAction(i, armorItem, nullptr, 632);
				second = new C_InventoryAction(slot, nullptr, armorItem);

				*g_Data.getLocalPlayer()->getArmor(i) = *emptyItemStack;
				*inv->getItemStack(slot) = *armorItem;

				manager->addInventoryAction(*first);
				manager->addInventoryAction(*second);

				delete first;
				delete second;

				first = new C_InventoryAction(armorList[0].m_slot, armorList[0].m_item, nullptr);
				second = new C_InventoryAction(i, nullptr, armorList[0].m_item, 632);

				*g_Data.getLocalPlayer()->getArmor(i) = *inv->getItemStack(armorList[0].m_slot);
				*inv->getItemStack(armorList[0].m_slot) = *emptyItemStack;

				manager->addInventoryAction(*first);
				manager->addInventoryAction(*second);

				delete first;
				delete second;
			}
			if (armorItem->item == nullptr) {
				*g_Data.getLocalPlayer()->getArmor(i) = *inv->getItemStack(armorList[0].m_slot);

				first = new C_InventoryAction(armorList[0].m_slot, armorList[0].m_item, nullptr);
				second = new C_InventoryAction(i, nullptr, armorList[0].m_item, 632);

				*inv->getItemStack(armorList[0].m_slot) = *emptyItemStack;

				manager->addInventoryAction(*first);
				manager->addInventoryAction(*second);

				delete first;
				delete second;
			}
		}
		armorList.clear();
	}
	armorList.clear();
}
