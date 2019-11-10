#include "setoffhandCommand.h"
#include <vector>
#include <string>



setoffhandCommand::setoffhandCommand() : IMCCommand("setoffhand", "Puts any item into offhand", "<ItemName> <count> <itemData>")
{
	registerAlias("soh");
}


setoffhandCommand::~setoffhandCommand()
{
}

bool setoffhandCommand::execute(std::vector<std::string>* args)
{
	assertTrue(args->size() > 2);
	int itemId = 0;
	char  count = assertInt(args->at(2));
	char itemData = 0;
	if (args->size() > 3)
		itemData = assertInt(args->at(3));
	try
	{
		itemId = std::stoi(args->at(1));
	}
	catch (const std::invalid_argument&)
	{
	}

	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_BlockLegacy* blockItem = nullptr;
	C_Item* itemItem = nullptr;
	C_ItemStack* yot = nullptr;

	static uintptr_t** VanillaBlocks__mDirtPtr = 0x0;
	static uintptr_t** VanillaItems__mShovel_ironPtr = 0x0;

	using getItemFromId_t = C_Item***(__fastcall*)(void*, int itemID);
	static getItemFromId_t  getItemFromId = reinterpret_cast<getItemFromId_t>(Utils::FindSignature("40 53 48 83 EC ?? 48 8B D9 66 85 D2 0F 84 ?? ?? ?? ?? 44 0F BF C2 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 44 89 44 24 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 41 0F B6 D0 48 33 D0 0F B6 44 24 ??"));

	if (VanillaBlocks__mDirtPtr == 0x0) {
		uintptr_t sigOffset = Utils::FindSignature("48 8B 05 ?? ?? ?? ?? 48 8B 08 48 8D 42 FF ?? ?? ?? ?? ?? ?? 49 3B C0");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		VanillaBlocks__mDirtPtr = reinterpret_cast<uintptr_t * *>(sigOffset + offset + /*length of instruction*/ 7);
		if (VanillaBlocks__mDirtPtr == 0x0 || sigOffset == 0x0)
			logF("VanillaBlocks sig not working!!!");
	}

	if (VanillaItems__mShovel_ironPtr == 0x0) {
		uintptr_t sigOffset = Utils::FindSignature("48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 90 ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 90 ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 90 ?? ?? ?? ?? ?? ?? ?? E8");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		VanillaItems__mShovel_ironPtr = reinterpret_cast<uintptr_t * *>(sigOffset + offset + /*length of instruction*/ 7);
		if (VanillaItems__mShovel_ironPtr == 0x0 || sigOffset == 0x0)
			logF("VanillaItems sig not working!!!");
	}

	if (itemId == 0)
	{
		if (VanillaBlocks__mDirtPtr != nullptr)
		{
			for (int i = 0; i < 465; i++)
			{
				if (VanillaBlocks__mDirtPtr[i] != nullptr &&
					strcmp(reinterpret_cast<C_BlockLegacy*>(VanillaBlocks__mDirtPtr[i][0])->name.getText(), args->at(1).c_str()) == 0)
				{
					blockItem = reinterpret_cast<C_BlockLegacy*>(VanillaBlocks__mDirtPtr[i][0]);
					break;
				}
			}
		}

		if (VanillaItems__mShovel_ironPtr != nullptr && blockItem == nullptr)
		{
			for (int i = 0; i < 234; i++)
			{
				if (VanillaItems__mShovel_ironPtr[i] != nullptr &&
					strcmp(reinterpret_cast<C_Item*>(VanillaItems__mShovel_ironPtr[i][0])->name.getText(), args->at(1).c_str()) == 0)
				{
					itemItem = reinterpret_cast<C_Item*>(VanillaItems__mShovel_ironPtr[i][0]);
					break;
				}
			}
		}
	}
	else
	{
		void* ItemPtr = malloc(0x8);
		C_Item*** cStack = getItemFromId(ItemPtr, itemId);
		if (**cStack == NULL)
		{
			clientMessageF("%sInvalid item ID!", RED);
			return true;
		}
		yot = new C_ItemStack(***cStack, count, itemData);
		g_Data.getLocalPlayer()->setOffhandSlot(yot);
		clientMessageF("%sSet item as offhand!", BLUE);
		free(ItemPtr);
		return true;
	}

	if (blockItem == nullptr && itemItem == nullptr)
	{
		clientMessageF("%sInvalid Item!", RED);
		return true;
	}
	else if (blockItem != nullptr)
	{
		if (itemData != 0)
			yot = new C_ItemStack(*blockItem, count);
		else
		{
			void* ItemPtr = malloc(0x8);
			C_Item*** cStack = getItemFromId(ItemPtr, blockItem->blockId);
			yot = new C_ItemStack(***cStack, count, itemData);
			free(ItemPtr);
		}
	}
	else
		yot = new C_ItemStack(*itemItem, count, itemData);

	if (yot != nullptr)
		yot->count = count;

	g_Data.getLocalPlayer()->setOffhandSlot(yot);
	clientMessageF("%sSet item as offhand!", BLUE);
	return true;
}
