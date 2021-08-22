#include "SetOffhandCommand.h"

#include "../../../SDK/Tag.h"
#include "../../../Utils/Utils.h"

SetOffhandCommand::SetOffhandCommand() : IMCCommand("setoffhand", "Spawn items in ur offhand", "<itemName> <count> <itemData>") {
	registerAlias("soh");
}

SetOffhandCommand::~SetOffhandCommand() {
}

bool SetOffhandCommand::execute(std::vector<std::string>* args) {
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
		TextHolder tempText(args->at(1));
		std::unique_ptr<void*> ItemPtr = std::make_unique<void*>();
		std::unique_ptr<void*> buffer = std::make_unique<void*>();
		C_Item*** cStack = ItemRegistry::lookUpByName(ItemPtr.get(), buffer.get(), tempText);
		if (*cStack == nullptr) {
			clientMessageF("%sInvalid item name!", RED);
			return true;
		}
		yot = new C_ItemStack(***cStack, count, itemData);
	} else {
		std::unique_ptr<void*> ItemPtr = std::make_unique<void*>();
		C_Item*** cStack = ItemRegistry::getItemFromId(ItemPtr.get(), itemId);
		if (cStack == nullptr || *cStack == nullptr || **cStack == nullptr) {
			clientMessageF("%sInvalid item ID!", RED);
			return true;
		}
		yot = new C_ItemStack(***cStack, count, itemData);
	}

	if (yot != nullptr)
		yot->count = count;

	if (args->size() > 4) {
		std::string tag = Utils::getClipboardText();
		if (tag.size() > 1 && tag.front() == MojangsonToken::COMPOUND_START.getSymbol() && tag.back() == MojangsonToken::COMPOUND_END.getSymbol()) {
			yot->setUserData(std::move(Mojangson::parseTag(tag)));
		}
	}

	ItemDescriptor* desc = nullptr;
	desc = new ItemDescriptor((*yot->item)->itemId, itemData);

	C_InventoryAction* firstAction = nullptr;
	C_InventoryAction* secondAction = nullptr;

	firstAction = new C_InventoryAction(0, desc, nullptr, yot, nullptr, count, 507, 99999);

	transactionManager->addInventoryAction(*firstAction);

	delete firstAction;
	delete desc;
	g_Data.getLocalPlayer()->setOffhandSlot(yot);

	clientMessageF("%sSuccessfully set item to offhand!", GREEN);
	return true;
}
