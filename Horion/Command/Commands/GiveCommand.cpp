#include "GiveCommand.h"
#include "../../../Utils/Utils.h"
#include "../../../SDK/Tag.h"

GiveCommand::GiveCommand() : IMCCommand("give", "spawn items", "<itemName> <count> <itemData> <NBT>") {
}

GiveCommand::~GiveCommand() {
}

bool GiveCommand::execute(std::vector<std::string>* args) {
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
		C_Item*** cStack = ItemRegistry::getItemFromId(ItemPtr, itemId);
		if (cStack == nullptr || *cStack == nullptr || **cStack == nullptr) {
			clientMessageF("%sInvalid item ID!", RED);
			return true;
		}
		yot = new C_ItemStack(***cStack, count, itemData);
		free(ItemPtr);
	}

	if (yot != nullptr)
		yot->count = count;

	int slot = inv->getFirstEmptySlot();

	if (args->size() > 4) {
		std::string tag = Utils::getClipboardText();
		if (tag.size() > 1 && tag.front() == MojangsonToken::COMPOUND_START.getSymbol() && tag.back() == MojangsonToken::COMPOUND_END.getSymbol()) {
			yot->setUserData(std::move(Mojangson::parseTag(tag)));
		}
	}

	C_InventoryAction* firstAction = nullptr;
	C_InventoryAction* secondAction = nullptr;
	firstAction = new C_InventoryAction(0, yot, nullptr, 507, 99999);
	secondAction = new C_InventoryAction(slot, nullptr, yot);

	transactionManager->addInventoryAction(*firstAction);
	transactionManager->addInventoryAction(*secondAction);

	delete firstAction;
	delete secondAction;

	inv->addItemToFirstEmptySlot(yot);

	clientMessageF("%sSuccessfully given item!", GREEN);
	return true;
}
