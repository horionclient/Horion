#include "setoffhandCommand.h"
#include "../../../Utils/Utils.h"
#include <string>
#include <vector>

setoffhandCommand::setoffhandCommand() : IMCCommand("setoffhand", "Puts any item into offhand", "<ItemName> <count> <itemData>") {
	registerAlias("soh");
}

setoffhandCommand::~setoffhandCommand() {
}

bool setoffhandCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() > 2);

	int itemId = 0;
	char count = static_cast<char>(assertInt(args->at(2)));
	char itemData = 0;
	if (args->size() > 3)
		itemData = static_cast<char>(assertInt(args->at(3)));

	try {
		itemId = std::stoi(args->at(1));
	} catch (const std::invalid_argument&) {
	}

	C_Inventory* inv = g_Data.getLocalPlayer()->getSupplies()->inventory;
	C_ItemStack* yot = nullptr;
	auto transactionManager = g_Data.getLocalPlayer()->getTransactionManager();
	
	if (itemId == 0) {
		void* ItemPtr = malloc(0x8);
		void* idk = malloc(0x0);
		TextHolder tempText(args->at(1));
		C_Item*** cStack = ItemRegistry::lookUpByName(ItemPtr, idk, tempText);
		if (*cStack == nullptr) {
			clientMessageF("%sInvalid item name!", RED);
			return true;
		}
		yot = new C_ItemStack(***cStack, count, itemData);
		free(ItemPtr);
		free(idk);
	} else {
		void* ItemPtr = malloc(0x8);
		C_Item*** cStack =	ItemRegistry::getItemFromId(ItemPtr, itemId);
		if (**cStack == NULL) {
			clientMessageF("%sInvalid item ID!", RED);
			return true;
		}
		yot = new C_ItemStack(***cStack, count, itemData);
		free(ItemPtr);
	}

	if (yot != nullptr)
		yot->count = count;

	g_Data.getLocalPlayer()->setOffhandSlot(yot);
	clientMessageF("%sSet item as offhand!", BLUE);
	return true;
}
