#include "DupeCommand.h"

DupeCommand::DupeCommand() : IMCCommand("dupe", "Duplicates the item in hand", "")
{
}

DupeCommand::~DupeCommand()
{
}

bool DupeCommand::execute(std::vector<std::string>* args)
{
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	int selectedSlot = supplies->selectedHotbarSlot;
	C_ItemStack* item = inv->getItemStack(selectedSlot);

	g_Data.getLocalPlayer()->setOffhandSlot(item); // Items with NBT tags don't really work well with normal item giving method

	clientMessageF("%sSuccessfully duplicated the item!", GREEN);
	return true;
}
