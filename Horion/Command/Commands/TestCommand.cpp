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
	logF("player_supplies: %llX", supplies);
	C_Inventory* inv = supplies->inventory;
	logF("supplies_inventory: %llX", inv);
	__debugbreak();
	uintptr_t item = reinterpret_cast<uintptr_t>(inv->getItem(0));
	logF("inventory_item0: %llX", item);
	return true;
}
