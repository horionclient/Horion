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

	C_MobEquipmentPacket* mob = new C_MobEquipmentPacket(g_Data.getLocalPlayer()->entityRuntimeId, *inv->getItemStack(5)
		, 5, 5);

	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(mob);

	C_ItemUseInventoryTransaction* complexTransaction = new C_ItemUseInventoryTransaction();
	C_InventoryTransactionPacket* packet = new C_InventoryTransactionPacket();

	complexTransaction->pos = *g_Data.getLocalPlayer()->getPos();
	complexTransaction->slot = 5;
	complexTransaction->blockSide = g_Data.getLocalPlayer()->pointingAt->blockSide;
	complexTransaction->blockPos = g_Data.getLocalPlayer()->pointingAt->block;

	complexTransaction->item = *inv->getItemStack(5);

	packet->complexTransaction = complexTransaction;

	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(packet);


	delete packet;
	delete mob;

	mob = new C_MobEquipmentPacket(g_Data.getLocalPlayer()->entityRuntimeId,
		*inv->getItemStack(supplies->selectedHotbarSlot)
		, supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);

	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(mob);

	delete mob;
	return true;
}
