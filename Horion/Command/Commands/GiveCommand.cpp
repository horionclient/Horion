#include "GiveCommand.h"

#include "../../../SDK/Tag.h"
#include "../../../Utils/Utils.h"

GiveCommand::GiveCommand() : IMCCommand("give", "spawn items", "<itemName> <count> [itemData] [NBT]") {
}

GiveCommand::~GiveCommand() {
}

bool GiveCommand::execute(std::vector<std::string> *args) {
	assertTrue(args->size() > 2);

	int itemId = 0;
	uint32_t fullCount = static_cast<uint32_t>(assertInt(args->at(2)));
	unsigned int stackCount = fullCount / 64;  // Get the amount of stacks we have.
	char count = fullCount % 64;               // Get the amount we have left.
	char itemData = 0;
	if (args->size() > 3) {
		itemData = static_cast<char>(assertInt(args->at(3)));
	}

	try {
		itemId = std::stoi(args->at(1));
	} catch (const std::invalid_argument &) {
	}

	//clientMessageF("%sDEBUG:%s Will give %d stacks!", RED, GREEN, stackCount);
	//clientMessageF("%sDEBUG:%s Will give %d as a remainder!", RED, GREEN, count);

	// Give us all the stacks of the items we want.
	for (unsigned int i = 0; i < stackCount; i++) {
		//clientMessageF("%sDEBUG:%s Giving stack %d of items!", RED, GREEN, i + 1);
		std::string tag;
		bool success = false;

		if (args->size() > 4) {
			std::string tag = Utils::getClipboardText();
		}
		if (itemId == 0) {
			TextHolder tempText(args->at(1));
			success = giveItem(64, tempText, itemData, tag);
		} else {
			success = giveItem(64, itemId, itemData, tag);
		}
		// If one of these fail. Then something went wrong.
		// Return to prevent a possible spam of error messages.
		if (!success) return true;
	}

	// Now give us our remainder.
	if (count >= 1) {
		//clientMessageF("%sDEBUG:%s Giving remaining %d items!", RED, GREEN, count);
		std::string tag;
		bool success = false;

		if (args->size() > 4) {
			std::string tag = Utils::getClipboardText();
		}
		if (itemId == 0) {
			TextHolder tempText(args->at(1));
			success = giveItem(count, tempText, itemData, tag);
		} else {
			success = giveItem(count, itemId, itemData, tag);
		}
		if (!success) return true;
	}
	C_Inventory *inv = g_Data.getLocalPlayer()->getSupplies()->inventory;
	C_ItemStack *item = g_Data.getLocalPlayer()->getSelectedItem();
	if (args->size() > 4) {
		std::string tag;
		tag = Utils::getClipboardText();
		if (args->size() > 4) {
			g_Data.getLocalPlayer()->getTransactionManager()->addInventoryAction(C_InventoryAction(0, nullptr, nullptr, item, nullptr, 1, 507, 99999));
		}

		if (tag.size() > 1 && tag.front() == MojangsonToken::COMPOUND_START.getSymbol() && tag.back() == MojangsonToken::COMPOUND_END.getSymbol()) {
			if (args->size() > 4) {
				item->setUserData(std::move(Mojangson::parseTag(tag)));
			}
		} else {
			clientMessageF("%sInvalid NBT tag!", RED);
			return true;
		}

		if (args->size() > 4) {
			g_Data.getLocalPlayer()->getTransactionManager()->addInventoryAction(C_InventoryAction(0, nullptr, nullptr, item, nullptr, 1, 507, 99999));
		}
		clientMessageF("%s%s", GREEN, "Successfully loaded mojangson !");
	}
	if (args->size() > 4) {
		C_InventoryAction *firstAction = nullptr;
		auto transactionMan = g_Data.getLocalPlayer()->getTransactionManager();
		firstAction = new C_InventoryAction(0, item, nullptr, 507, 99999);
		transactionMan->addInventoryAction(*firstAction);
		inv->addItemToFirstEmptySlot(item);
	}

	clientMessageF("%sSuccessfully gave items!", GREEN);
	return true;
}

bool GiveCommand::giveItem(uint8_t count, int itemId, uint8_t itemData, std::string &tag) {
	C_Inventory *inv = g_Data.getLocalPlayer()->getSupplies()->inventory;
	C_ItemStack *itemStack = nullptr;
	auto transactionManager = g_Data.getLocalPlayer()->getTransactionManager();

	std::unique_ptr<void *> ItemPtr = std::make_unique<void *>();
	C_Item ***cStack = ItemRegistry::getItemFromId(ItemPtr.get(), itemId);
	if (cStack == nullptr || *cStack == nullptr || **cStack == nullptr) {
		clientMessageF("%sInvalid item ID!", RED);
		return false;
	}
	itemStack = new C_ItemStack(***cStack, count, itemData);

	if (itemStack != nullptr) {
		itemStack->count = count;
	}

	int slot = inv->getFirstEmptySlot();

	if (tag.size() > 1 && tag.front() == MojangsonToken::COMPOUND_START.getSymbol() && tag.back() == MojangsonToken::COMPOUND_END.getSymbol()) {
		//itemStack->setUserData(std::move(Mojangson::parseTag(tag)));
		itemStack->fromTag(*Mojangson::parseTag(tag));
	}

	ItemDescriptor *desc = new ItemDescriptor((*itemStack->item)->itemId, itemData);

	// If we add the second action, Only one stack will come through for some reason.
	// Otherwise all stacks will come through but will be buggy till dropped or
	// till the world is saved then reloaded.

	C_InventoryAction *firstAction = new C_InventoryAction(slot, desc, nullptr, itemStack, nullptr, count, 507, 99999);
	//C_InventoryAction *secondAction = new C_InventoryAction(slot, nullptr, desc, nullptr, itemStack, count);

	transactionManager->addInventoryAction(*firstAction);
	//transactionManager->addInventoryAction(*secondAction);

	delete firstAction;
	//delete secondAction;
	delete desc;

	inv->addItemToFirstEmptySlot(itemStack);
	return true;
}

bool GiveCommand::giveItem(uint8_t count, TextHolder &text, uint8_t itemData, std::string &tag) {
	C_Inventory *inv = g_Data.getLocalPlayer()->getSupplies()->inventory;
	C_ItemStack *itemStack = nullptr;
	auto transactionManager = g_Data.getLocalPlayer()->getTransactionManager();

	std::unique_ptr<void *> ItemPtr = std::make_unique<void *>();
	std::unique_ptr<void *> buffer = std::make_unique<void *>();
	C_Item ***cStack = ItemRegistry::lookUpByName(ItemPtr.get(), buffer.get(), text);
	if (*cStack == nullptr) {
		clientMessageF("%sInvalid item name!", RED);
		return false;
	}
	itemStack = new C_ItemStack(***cStack, count, itemData);

	if (itemStack != nullptr) {
		itemStack->count = count;
	}

	int slot = inv->getFirstEmptySlot();

	if (tag.size() > 1 && tag.front() == MojangsonToken::COMPOUND_START.getSymbol() && tag.back() == MojangsonToken::COMPOUND_END.getSymbol()) {
		//itemStack->setUserData(std::move(Mojangson::parseTag(tag)));
		itemStack->fromTag(*Mojangson::parseTag(tag));
	}

	ItemDescriptor *desc = new ItemDescriptor((*itemStack->item)->itemId, itemData);

	// If we add the second action, Only one stack will come through for some reason.
	// Otherwise all stacks will come through but will be buggy till dropped or
	// till the world is saved then reloaded.

	C_InventoryAction *firstAction = new C_InventoryAction(slot, desc, nullptr, itemStack, nullptr, count, 507, 99999);
	//C_InventoryAction *secondAction = new C_InventoryAction(slot, nullptr, desc, nullptr, itemStack, count);

	transactionManager->addInventoryAction(*firstAction);
	//transactionManager->addInventoryAction(*secondAction);

	delete firstAction;
	//delete secondAction;
	delete desc;

	inv->addItemToFirstEmptySlot(itemStack);
	return true;
}
