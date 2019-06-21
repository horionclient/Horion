#include "TestCommand.h"



TestCommand::TestCommand() : ICommand("test", "Test for Debugging purposes", "")
{
}


TestCommand::~TestCommand()
{
}

bool TestCommand::execute(std::vector<std::string>* args)
{

	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_ItemStack* item = inv->getItemStack(0);
	C_Item* u = *item->item;
	g_Data.getLocalPlayer()->setOffhandSlot(item);
	logF("clinet: %llX",supplies);
	return true;
}
