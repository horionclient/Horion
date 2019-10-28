#include "OffhandFixer.h"



OffhandFixer::OffhandFixer() : IModule(0x0, PLAYER)
{
}


OffhandFixer::~OffhandFixer()
{
}

const char* OffhandFixer::getModuleName()
{
	return ("OffhandFixer");
}

void OffhandFixer::onTick(C_GameMode* gm) {
	int itemId = 513;
	char count = 1;
	char itemData = 0;

	using getItemFromId_t = C_ItemStack * (__fastcall*)(void*, int itemID);
	static getItemFromId_t  getItemFromId = reinterpret_cast<getItemFromId_t>(Utils::FindSignature("40 53 48 83 EC 30 48 8B D9 66 85 D2 0F 84 ?? ?? ?? ?? 44 0F BF CA 48 8D 4C 24 ?? 44 89 4C 24 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 66 0F 1F 44 00 00 0F B6 01 48 8D 49 ??"));

	void* ItemPtr = malloc(0x8);
	C_ItemStack* cStack = getItemFromId(ItemPtr, itemId);
	C_ItemStack* sohItem = reinterpret_cast<C_ItemStack*>(malloc(0x88));
	sohItem = new C_ItemStack(*cStack->item, count, itemData);
	g_Data.getLocalPlayer()->setOffhandSlot(sohItem);
}
