#include "EnchantCommand.h"



EnchantCommand::EnchantCommand() : ICommand("enchant", "Enchants items", "<enchantment> [level]")
{
}


EnchantCommand::~EnchantCommand()
{
}

bool EnchantCommand::execute(std::vector<std::string>* args)
{
	assertTrue(args->size() > 1);
	int enchantId = assertInt(args->at(1));
	int enchantLevel = 1;
	if (args->size() > 2)
		enchantLevel = assertInt(args->at(2));

	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_ItemStack* item = (inv->getItemStack(supplies->selectedHotbarSlot));

	using getEnchantsFromUserData_t = void(__fastcall*)(C_ItemStack*, void*  );
	using addEnchant_t              = bool(__fastcall*)(void*  , __int64);
	using saveEnchantsToUserData_t  = void(__fastcall*)(C_ItemStack*, void*  );

	static getEnchantsFromUserData_t getEnchantsFromUserData = 0x0;
	static addEnchant_t              addEnchant              = reinterpret_cast<addEnchant_t>(Utils::FindSignature("48 89 5C 24 ?? 48 89 54 24 ?? 57 48 83 EC ?? 48 8B DA 4C 8B C2"));
	static saveEnchantsToUserData_t  saveEnchantsToUserData  = reinterpret_cast<saveEnchantsToUserData_t>(Utils::FindSignature("40 57 48 83 EC ?? 48 C7 44 24 ?? FE FF FF FF 48 89 5C 24 ?? 48 8B FA 4C 8B C1 48 8B 01 48 85 C0"));
	
	if (getEnchantsFromUserData == 0x0) {
		uintptr_t sig = Utils::FindSignature("48 8D 95 ?? ?? ?? ?? 48 8B CB E8 ?? ?? ?? ?? 90 33 C0") + 11;
		int offset = *reinterpret_cast<int*>(sig);
		getEnchantsFromUserData = reinterpret_cast<getEnchantsFromUserData_t>(sig + offset + 4);
	}

	void* EnchantData = malloc(0x50);
	memset(EnchantData, 0x0, 0x50);

	getEnchantsFromUserData(item, EnchantData);

	__int64 enchantPair = ((__int64) enchantLevel << 32) | enchantId;

	if (addEnchant(EnchantData, enchantPair)) { // Upper 4 bytes = level, lower 4 bytes = enchant type
		saveEnchantsToUserData(item, EnchantData);
		__int64 proxy = reinterpret_cast<__int64>(g_Data.getLocalPlayer()->getSupplies());
		if (!*(uint8_t *)(proxy + 160))
			(*(void(__fastcall **)(unsigned long long, unsigned long long, C_ItemStack *))(**(unsigned long long **)(proxy + 168) + 56i64))(
				*(unsigned long long *)(proxy + 168),
				*(unsigned int *)(proxy + 16),
				item);// Player::selectItem

		g_Data.getLocalPlayer()->sendInventory();
		clientMessageF("%sEnchant successful!", GREEN);
	}
	else
		clientMessageF("%sEnchant failed, try using a lower enchant-level", RED);

	delete EnchantData;

	return true;
}
