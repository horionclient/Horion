#include "AutoArmor.h"


struct ArmorStruct
{
	ArmorStruct(C_ArmorItem* yot, int slot)
	{
		armor = yot;
		m_slot = slot;
		if (slot < 9)
			type = "hotbar_items";
		else
		{
			type = "inventory_items";
			m_slot -= 9;
		}
			
	}
	C_ArmorItem* armor = nullptr;
	std::string type;
	int m_slot = 0;
};

struct CompareArmorStruct
{
	bool operator() (ArmorStruct lhs, ArmorStruct rhs)
	{
		return (lhs.armor->getArmorValue() > rhs.armor->getArmorValue());
	}
};

std::vector<ArmorStruct> armorList;

AutoArmor::AutoArmor() : IModule(0x0, PLAYER)
{
}

AutoArmor::~AutoArmor()
{
}

const char* AutoArmor::getModuleName()
{
	return ("AutoArmor");
}

void AutoArmor::onTick(C_GameMode* gm) 
{
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	armorList.clear();

	for (int i = 0; i < 4; i++)
	{
		for (int n = 0; n < 36; n++)
		{
			C_ItemStack* stack = inv->getItemStack(n);
			if (stack->item != NULL && (*stack->item)->isArmor() && reinterpret_cast<C_ArmorItem*>(*stack->item)->ArmorSlot == i)
			{
				armorList.push_back(ArmorStruct(reinterpret_cast<C_ArmorItem*>(*stack->item),n));
			}
		}
		if (armorList.size() > 0 && g_Data.getLocalPlayer()->isInventoryClosed() == 0 && inventoryScreen != nullptr)
		{
			std::sort(armorList.begin(), armorList.end(), CompareArmorStruct());
			C_ItemStack* yot = inventoryScreen->_getItemStack(TextHolder(std::string("armor_items")), i);
			if (yot->item != nullptr && (*yot->item)->getArmorValue() < armorList[0].armor->getArmorValue())
			{
				inventoryScreen->handleAutoPlace(0x7FFFFFFF, "armor_items", i);
				inventoryScreen->handleAutoPlace(0x7FFFFFFF, armorList[0].type.c_str(), armorList[0].m_slot);
			}
			else if (yot->item == nullptr)
				inventoryScreen->handleAutoPlace(0x7FFFFFFF, armorList[0].type.c_str(), armorList[0].m_slot);
			armorList.clear();
		}
	}
	inventoryScreen = nullptr;
	armorList.clear();
}