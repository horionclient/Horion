#include "NbtCommand.h"

NbtCommand::NbtCommand() : IMCCommand("nbt", "read and write NBT tags to/from your clipboard (You have to point at an entity/block entity)", "<read/write>") {
}

NbtCommand::~NbtCommand() {
}

bool NbtCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() > 1);

	std::unique_ptr<CompoundTag> tag = std::make_unique<CompoundTag>();

	PointingStruct* pointingStruct = g_Data.getClientInstance()->getPointerStruct();
	C_BlockActor* blockActor = g_Data.getLocalPlayer()->region->getBlockEntity(pointingStruct->block);
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();
	C_ItemStack* item = g_Data.getLocalPlayer()->getSelectedItem();

	if (args->at(1) == "read") {
		if (pointingStruct->entityPtr != nullptr) {
			if(!(g_Data.getRakNetInstance()->serverIp.getTextLength() < 1)) {
				clientMessageF("%sNBT tags for mobs only works in local world!", RED);
				return true;
			}
			pointingStruct->entityPtr->save(tag.get());
			std::stringstream build;
			tag->write(build);
			Utils::setClipboardText(build.str());
			g_Data.getGuiData()->displayClientMessageF("%s%s", GREEN, "CompoundTag copied:");
			g_Data.getClientInstance()->getGuiData()->displayClientMessage(&build.str());
		} else if (blockActor != nullptr) {
			blockActor->save(tag.get());
			std::stringstream build;
			tag->write(build);
			Utils::setClipboardText(build.str());
			g_Data.getGuiData()->displayClientMessageF("%s%s", GREEN, "CompoundTag copied:");
			g_Data.getClientInstance()->getGuiData()->displayClientMessage(&build.str());
		} else if (item != nullptr && item->tag != nullptr) {
			std::stringstream build;
			item->tag->write(build);
			Utils::setClipboardText(build.str());
			g_Data.getGuiData()->displayClientMessageF("%s%s", GREEN, "CompoundTag copied:");
			g_Data.getClientInstance()->getGuiData()->displayClientMessage(&build.str());
		} else {
			clientMessageF("%sCouldn't find any NBT tags!", RED);
			return true;
		}
	} else if (args->at(1) == "write") {
		std::string tag = Utils::getClipboardText();

		C_InventoryAction* firstAction = nullptr;
		C_InventoryAction* secondAction = nullptr;

		{
			firstAction = new C_InventoryAction(supplies->selectedHotbarSlot, item, nullptr);
			secondAction = new C_InventoryAction(0, nullptr, item, 507, 99999);
			manager->addInventoryAction(*firstAction);
			manager->addInventoryAction(*secondAction);
			delete firstAction;
			delete secondAction;
		}

		if (tag.size() > 1 && tag.front() == MojangsonToken::COMPOUND_START.getSymbol() && tag.back() == MojangsonToken::COMPOUND_END.getSymbol()) {
			item->setUserData(std::move(Mojangson::parseTag(tag)));
		} else {
			clientMessageF("%sInvalid NBT tag!", RED);
			return true;
		}

		firstAction = new C_InventoryAction(0, item, nullptr, 507, 99999);
		secondAction = new C_InventoryAction(supplies->selectedHotbarSlot, nullptr, item);
		manager->addInventoryAction(*firstAction);
		manager->addInventoryAction(*secondAction);
		delete firstAction;
		delete secondAction;
	}

	return true;
}
