#include "TestCommand.h"



TestCommand::TestCommand() : ICommand("test", "Test for Debugging purposes", "")
{
}


TestCommand::~TestCommand()
{
}

bool TestCommand::execute(std::vector<std::string>* args)
{
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_Item* item = (inv->getItem(supplies->selectedHotbarSlot));

	// ItemStack::getEnchantsFromUserData
	// 48 8D 95 ?? ?? ?? ?? 48 8B CB E8 ?? ?? ?? ?? 90 33 C0
	//                                  ^^^^^^^^^^^ function call
	
	using getEnchantsFromUserData_t = void(__fastcall*)(C_Item*, void*);
	static getEnchantsFromUserData_t getEnchantsFromUserData = 0x0;
	if (getEnchantsFromUserData == 0x0) {
		uintptr_t sig = Utils::FindSignature("48 8D 95 ?? ?? ?? ?? 48 8B CB E8 ?? ?? ?? ?? 90 33 C0") + 11;
		int offset = *reinterpret_cast<int*>(sig);
		getEnchantsFromUserData = reinterpret_cast<getEnchantsFromUserData_t>(sig + offset + 4);
	}

	using addEnchant_t = bool(__fastcall*)(void*, __int64);
	static addEnchant_t addEnchant = reinterpret_cast<addEnchant_t>(Utils::FindSignature("48 89 5C 24 ?? 48 89 54 24 ?? 57 48 83 EC ?? 48 8B DA 4C 8B C2"));

	using saveEnchantsToUserData_t = void(__fastcall*)(C_Item*, void*);
	saveEnchantsToUserData_t saveEnchantsToUserData = reinterpret_cast<saveEnchantsToUserData_t>(Utils::FindSignature("40 57 48 83 EC ?? 48 C7 44 24 ?? FE FF FF FF 48 89 5C 24 ?? 48 8B FA 4C 8B C1 48 8B 01 48 85 C0"));

	void* alloc = malloc(0x50);
	memset(alloc, 0x0, 0x50);

	getEnchantsFromUserData(item, alloc);
	if (addEnchant(alloc, 0x7FF000000009)) { // Upper 4 bytes = level, lower 4 bytes = enchant type
		logF("Successful enchant add");

		saveEnchantsToUserData(item, alloc);
		__int64 proxy = reinterpret_cast<__int64>(g_Data.getLocalPlayer()->getSupplies()); 
		if (!*(uint8_t *)(proxy + 160))
			(*(void(__fastcall **)(unsigned long long, unsigned long long, C_Item *))(**(unsigned long long **)(proxy + 168) + 56i64))(
				*(unsigned long long *)(proxy + 168),
				*(unsigned int *)(proxy + 16),
				item);// Player::selectItem


		g_Data.getLocalPlayer()->getServerPlayer()->sendInventory();
	}else
		logF("Unsuccessful enchant add");


	delete alloc;
	
	return true;
}
