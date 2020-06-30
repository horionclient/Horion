#include "TestCommand.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/Utils.h"
#include <sstream>
#include "../../../SDK/Tag.h"

TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "") {
}

TestCommand::~TestCommand() {
}

void listEnts(C_Entity* ent, bool isValid) {
	int id = ent->getEntityTypeId();
	char* name = ent->getNameTag()->getText();
	logF("---------------");
	logF("Entity Name: %s", name);
	logF("Entity ID: %d", id);
	logF("---------------");
}

void showAimedBlockInfo() {
	PointingStruct* pointingStruct = g_Data.getClientInstance()->getPointerStruct();
	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(pointingStruct->block);
	int id = block->toLegacy()->blockId;
	char* name = block->toLegacy()->name.getText();
	logF("---------------");
	logF("Block Name: %s", name);
	logF("Block ID: %d", id);
	logF("---------------");
}

bool TestCommand::execute(std::vector<std::string>* args) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();

	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto transactionManager = g_Data.getLocalPlayer()->getTransactionManager();

	C_ItemStack* selectedItem = g_Data.getLocalPlayer()->getSelectedItem();

	auto slot = supplies->selectedHotbarSlot;

	C_InventoryAction actions[36 * 2];
	int numActions = 0;
	for(int i = slot + 1; i < 9; i++){
		auto itemInSlot = supplies->inventory->getItemStack(slot);
		//if(itemInSlot == nullptr || itemInSlot->item == nullptr || *itemInSlot->item == nullptr)
		{
			actions[numActions] = C_InventoryAction(slot, selectedItem, nullptr, 0, 0);
			actions[numActions + 1] = C_InventoryAction(i, nullptr, selectedItem, 0, 0);

			numActions += 2;
		}
	}

	//transactionManager->addInventoryAction(firstAction);

	C_InventoryTransactionPacket pk;
	pk.complexTransaction = new C_ComplexInventoryTransaction();
	pk.complexTransaction->actionType = 0;

	struct temp {
		__int64 next;
		char pad[0x18];
		C_InventoryAction* start;
		C_InventoryAction* end;
	};

	temp coolBean{};
	coolBean.start = &actions[0];
	coolBean.end = &actions[numActions];

	auto ptrBean = &coolBean;

	coolBean.next = reinterpret_cast<__int64>(&ptrBean);

	pk.complexTransaction->transac.ptr = reinterpret_cast<__int64>(&ptrBean);
	pk.numTransactions = 2;
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&pk);
	clientMessageF("%sSuccessfully given item! %i", GREEN, numActions);
	return true;
}
