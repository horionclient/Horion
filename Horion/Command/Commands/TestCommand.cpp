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

	C_ItemUseInventoryTransaction* transaction = new C_ItemUseInventoryTransaction();
	C_InventoryTransactionPacket* packet = new C_InventoryTransactionPacket();

	transaction->pos = *g_Data.getLocalPlayer()->getPos();
	transaction->slot = supplies->selectedHotbarSlot;
	transaction->blockSide = g_Data.getLocalPlayer()->pointingAt->blockSide;
	transaction->blockPos = g_Data.getLocalPlayer()->pointingAt->block;
	transaction->block = g_Data.getLocalPlayer()->region->getBlock(g_Data.getLocalPlayer()->pointingAt->block);
	transaction->item = inv->getItemStack(supplies->selectedHotbarSlot)->item;

	packet->transaction = transaction;

	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(packet);

	delete packet;
	delete transaction;
	return true;
}
