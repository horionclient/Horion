#include "setoffhandCommand.h"
#include <vector>
#include <string>



setoffhandCommand::setoffhandCommand() : IMCCommand("setoffhand", "Puts current item into offhand", "")
{
	registerAlias("soh");
}


setoffhandCommand::~setoffhandCommand()
{
}

bool setoffhandCommand::execute(std::vector<std::string>* args)
{
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	int selectedSlot = supplies->selectedHotbarSlot;
	C_ItemStack* item = (inv->getItemStack(selectedSlot));

	g_Data.getLocalPlayer()->setOffhandSlot(item);




	clientMessageF("%sSet item as offhand!", BLUE);
	return true;
}
