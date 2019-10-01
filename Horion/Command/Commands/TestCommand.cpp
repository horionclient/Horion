#include "TestCommand.h"


TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "")
{
}


TestCommand::~TestCommand()
{
}

bool TestCommand::execute(std::vector<std::string>* args)
{
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	uintptr_t o = (uintptr_t)(g_Data.getLocalPlayer());
	C_InventoryTransactionManager* manager = (C_InventoryTransactionManager*)(o+0x1B20);

	using oof = void(__fastcall*)(C_InventoryTransactionManager*, C_InventoryAction*);
	oof uiop = (oof)(0x7FF7EAF23760);


	C_InventoryAction *firtAction = new C_InventoryAction(0, nullptr, inv->getItemStack(supplies->selectedHotbarSlot), 255,2);
	C_InventoryAction *secondAction = new C_InventoryAction(supplies->selectedHotbarSlot, inv->getItemStack(supplies->selectedHotbarSlot), inv->getItemStack(supplies->selectedHotbarSlot));

	firtAction->sourceItem.count = 1;
	secondAction->sourceItem.count = 1;
	secondAction->targetItem.count = 1;

	uiop(manager, firtAction);
	uiop(manager, secondAction);

	return true;
}
