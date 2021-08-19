#include "DupeCommand.h"

DupeCommand::DupeCommand() : IMCCommand("dupe", "Duplicates the item in hand", "<count> <mode: give / offhand : 1/0>") {
	registerAlias("d");
}

DupeCommand::~DupeCommand() {
}

bool DupeCommand::execute(std::vector<std::string>* args) {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	auto transactionManager = g_Data.getLocalPlayer()->getTransactionManager();
	C_Inventory* inv = supplies->inventory;
	int selectedSlot = supplies->selectedHotbarSlot;
	C_ItemStack* item = inv->getItemStack(selectedSlot);
	int count = item->count;
	bool isGive = true;
	if (args->size() > 1)
		item->count = assertInt(args->at(1));
	if (args->size() > 2)
		isGive = static_cast<bool>(assertInt(args->at(2)));
	if (isGive) {
		C_InventoryAction* firstAction = nullptr;
		C_InventoryAction* secondAction = nullptr;
		firstAction = new C_InventoryAction(0, item, nullptr, 507, 99999);
		transactionManager->addInventoryAction(*firstAction);
		inv->addItemToFirstEmptySlot(item);
	} else {
		C_InventoryAction* pp = nullptr;
		pp = new C_InventoryAction(0, item, nullptr, 507, 99999);
		transactionManager->addInventoryAction(*pp);
		g_Data.getLocalPlayer()->setOffhandSlot(item);
	}
	if (args->size() > 1)
		item->count = count;
		clientMessageF("%sSuccessfully duplicated the item!", GREEN);
	return true;
}
