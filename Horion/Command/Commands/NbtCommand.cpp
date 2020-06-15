#include "NbtCommand.h"
#include "../../../Utils/Utils.h"
#include "../../../SDK/Tag.h"

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

	if (args->at(1) == "read" || args->at(1) == "save") {
		std::stringstream build;

		if (args->at(1) == "save" && item != nullptr) {
			CompoundTag* boy = new CompoundTag();
			item->save(&boy);
			boy->write(build);
			delete boy;
		} else {
			if (pointingStruct->entityPtr != nullptr) {
				if (!(g_Data.getRakNetInstance()->serverIp.getTextLength() < 1)) {
					clientMessageF("%sNBT tags for mobs only works in local world!", RED);
					return true;
				}
				pointingStruct->entityPtr->save(tag.get());
				tag->write(build);
			} else if (blockActor != nullptr) {
				blockActor->save(tag.get());
				tag->write(build);
			} else if (item != nullptr && item->tag != nullptr) {
				item->tag->write(build);
			} else {
				clientMessageF("%sCouldn't find NBT tags!", RED);
				return true;
			}
		}

		auto builtStr = build.str();
		Utils::setClipboardText(builtStr);
		clientMessageF("%s%s", GREEN, "CompoundTag copied:");
		clientMessageF(builtStr.c_str());
	} else if ((args->at(1) == "write" || args->at(1) == "load") && item) {
		std::string tag = Utils::getClipboardText();

		{
			manager->addInventoryAction(C_InventoryAction(supplies->selectedHotbarSlot, item, nullptr));
			manager->addInventoryAction(C_InventoryAction(0, nullptr, item, 507, 99999));
		}

		if (tag.size() > 1 && tag.front() == MojangsonToken::COMPOUND_START.getSymbol() && tag.back() == MojangsonToken::COMPOUND_END.getSymbol()) {
			if (args->at(1) == "write")
				item->setUserData(std::move(Mojangson::parseTag(tag)));
			else if (args->at(1) == "load") {
				item->fromTag(*Mojangson::parseTag(tag).get());
				item->count = 64;
			}
		} else {
			clientMessageF("%sInvalid NBT tag!", RED);
			return true;
		}

		{
			manager->addInventoryAction(C_InventoryAction(0, item, nullptr, 507, 99999));
			manager->addInventoryAction(C_InventoryAction(supplies->selectedHotbarSlot, nullptr, item));
		}

		clientMessageF("%s%s", GREEN, "Successfully loaded mojangson !");
	} else {
		clientMessageF("%s%s", RED, "Couldn't execute command correctly");
	}

	return true;
}
