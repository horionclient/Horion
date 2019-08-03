#include "GiveCommand.h"



GiveCommand::GiveCommand() : IMCCommand("give", "spawn items", "<itemID> <count> <itemData>") 
{
}


GiveCommand::~GiveCommand()
{
}

bool GiveCommand::execute(std::vector<std::string>* args)
{
	assertTrue(args->size() > 2);
	int itemId = assertInt(args->at(1));
	int  count = assertInt(args->at(2));
	int itemData = 0;
	if (args->size() > 3)
		itemData = assertInt(args->at(3));

	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	using getItemFromId_t = C_ItemStack*(__fastcall*)(void*,int itemID);
	using ItemStackContructor_t = C_ItemStack*(__fastcall*)(void*,C_Item*,int count,int damage);
	
	static getItemFromId_t  getItemFromId = reinterpret_cast<getItemFromId_t>(Utils::FindSignature("40 53 48 83 EC 30 48 8B D9 66 85 D2 0F 84 ?? ?? ?? ?? 44 0F BF CA 48 8D 4C 24 ?? 44 89 4C 24 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 66 0F 1F 44 00 00 0F B6 01 48 8D 49 ??"));
	static ItemStackContructor_t  ItemStackContructor = reinterpret_cast<ItemStackContructor_t>(Utils::FindSignature("40 53 48 83 EC 40 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B D9 48 89 4C 24 ?? 33 C0 48 89 01 48 89 41 ?? 48 89 41 ?? C7 41 ?? ?? ?? ?? ?? 48 89 41 ?? 48 ?? ?? ?? ?? ?? ?? ?? C6 41 ?? 01 48 89 41 ?? 48 89 41 ?? 48 89 41 ?? 48 89 41 ?? 48 89 41 ?? 48 89 41 ?? 48 89 41 ?? 48 89 41 ?? 48 89 41 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 44 24 ?? E8 ?? ?? ?? ?? 90 48 8B C3 48 83 C4 40 5B C3 CC CC CC CC CC CC 40 53 48 83 EC 40 48"));
	
	void* ItemPtr = malloc(0x8);
	memset(ItemPtr, 0x0, 0x8);
	C_ItemStack* yeet = getItemFromId(ItemPtr, itemId);

	void* ItemStackObject = malloc(0x50);
	memset(ItemStackObject, 0x0, 0x50);

	C_ItemStack* yot = ItemStackContructor(ItemStackObject,*yeet->item, count, itemData);

	inv->addItemToFirstEmptySlot(yot);
	clientMessageF("%ssuccessfully given item!", GREEN);
	return true;
}
