#include "setoffhandCommand.h"
#include <vector>
#include <string>



setoffhandCommand::setoffhandCommand() : IMCCommand("setoffhand", "Puts any item into offhand", "<itemID> <count> <itemData>")
{
	registerAlias("soh");
}


setoffhandCommand::~setoffhandCommand()
{
}

bool setoffhandCommand::execute(std::vector<std::string>* args)
{
	assertTrue(args->size() > 2);
	int itemId = assertInt(args->at(1));
	int  count = assertInt(args->at(2));
	int itemData = 0;
	if (args->size() > 3)
		itemData = assertInt(args->at(3));

	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	using getItemFromId_t = C_ItemStack * (__fastcall*)(void*, int itemID);
	static getItemFromId_t  getItemFromId = reinterpret_cast<getItemFromId_t>(Utils::FindSignature("40 53 48 83 EC 30 48 8B D9 66 85 D2 0F 84 ?? ?? ?? ?? 44 0F BF CA 48 8D 4C 24 ?? 44 89 4C 24 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 66 0F 1F 44 00 00 0F B6 01 48 8D 49 ??"));

	void* ItemPtr = malloc(0x8);
	C_ItemStack* yeet = getItemFromId(ItemPtr, itemId);

	if (yeet->item == NULL)
	{
		clientMessageF("%sInvalid item ID!", RED);
		return true;
	}


	C_ItemStack* yot = reinterpret_cast<C_ItemStack*>(malloc(0x88));
	yot->ItemStackConstructor(*yeet->item, count, itemData);

	g_Data.getLocalPlayer()->setOffhandSlot(yot);
	clientMessageF("%sSet item as offhand!", BLUE);
	return true;
}
