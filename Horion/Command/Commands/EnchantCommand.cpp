#include "EnchantCommand.h"



EnchantCommand::EnchantCommand() : IMCCommand("enchant", "Enchants items", "<enchantment> [level]")
{
}


EnchantCommand::~EnchantCommand()
{
}

bool EnchantCommand::execute(std::vector<std::string>* args)
{
	assertTrue(args->size() > 1); // this line stops the command from been run if the proper args are not given

	int enchantId = 0;
	int enchantLevel = 32767;
	if (args->size() > 2)
		enchantLevel = assertInt(args->at(2));

	if (strcmp(args->at(1).c_str(), "Protection") == 0)
		enchantId = 0;

	if (strcmp(args->at(1).c_str(), "Fire_Protection") == 0)
		enchantId = 1;

	if (strcmp(args->at(1).c_str(), "Feather_Falling") == 0)
		enchantId = 2;

	if (strcmp(args->at(1).c_str(), "Blast_Protection") == 0)
		enchantId = 3;

	if (strcmp(args->at(1).c_str(), "Projectile_Protection") == 0)
		enchantId = 4;

	if (strcmp(args->at(1).c_str(), "Thorns") == 0)
		enchantId = 5;

	if (strcmp(args->at(1).c_str(), "Respiration") == 0)
		enchantId = 6;

	if (strcmp(args->at(1).c_str(), "Depth_Strider") == 0)
		enchantId = 7;

	if (strcmp(args->at(1).c_str(), "Aqua_Infinity") == 0)
		enchantId = 8;

	if (strcmp(args->at(1).c_str(), "Frost_walker") == 0)
		enchantId = 25;

	if (strcmp(args->at(1).c_str(), "Sharpness") == 0)
		enchantId = 9;

	if (strcmp(args->at(1).c_str(), "Smite") == 0)
		enchantId = 10;

	if (strcmp(args->at(1).c_str(), "Bane_of_Arthropods") == 0)
		enchantId = 11;

	if (strcmp(args->at(1).c_str(), "Knockback") == 0)
		enchantId = 12;

	if (strcmp(args->at(1).c_str(), "Fire_Aspect") == 0)
		enchantId = 13;

	if (strcmp(args->at(1).c_str(), "Looting") == 0)
		enchantId = 14;

	if (strcmp(args->at(1).c_str(), "Channeling") == 0)
		enchantId = 32;

	if (strcmp(args->at(1).c_str(), "Impaling") == 0)
		enchantId = 29;

	if (strcmp(args->at(1).c_str(), "Loyalty") == 0)
		enchantId = 31;

	if (strcmp(args->at(1).c_str(), "Riptide") == 0)
		enchantId = 30;

	if (strcmp(args->at(1).c_str(), "SilkTouch") == 0)
		enchantId = 16;

	if (strcmp(args->at(1).c_str(), "Fortune") == 0)
		enchantId = 18;

	if (strcmp(args->at(1).c_str(), "Unbreaking") == 0)
		enchantId = 17;

	if (strcmp(args->at(1).c_str(), "Efficiency") == 0)
		enchantId = 15;

	if (strcmp(args->at(1).c_str(), "Mending") == 0)
		enchantId = 26;

	if (strcmp(args->at(1).c_str(), "Power") == 0)
		enchantId = 19;

	if (strcmp(args->at(1).c_str(), "Punch") == 0)
		enchantId = 20;

	if (strcmp(args->at(1).c_str(), "Flame") == 0)
		enchantId = 21;

	if (strcmp(args->at(1).c_str(), "Infinity") == 0)
		enchantId = 22;

	if (strcmp(args->at(1).c_str(), "Multishot") == 0)
		enchantId = 33;

	if (strcmp(args->at(1).c_str(), "Quick_Charge") == 0)
		enchantId = 35;

	if (strcmp(args->at(1).c_str(), "Piercing") == 0)
		enchantId = 34;

	if (strcmp(args->at(1).c_str(), "Luck_of_Sea") == 0)
		enchantId = 23;

	if (strcmp(args->at(1).c_str(), "Lure") == 0)
		enchantId = 24;

	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	int selectedSlot = supplies->selectedHotbarSlot;
	C_ItemStack* item = (inv->getItemStack(selectedSlot));

	using getEnchantsFromUserData_t = void(__fastcall*)(C_ItemStack*, void*);
	using addEnchant_t              = bool(__fastcall*)(void*, __int64);
	using saveEnchantsToUserData_t  = void(__fastcall*)(C_ItemStack*, void*);

	static getEnchantsFromUserData_t getEnchantsFromUserData = 0x0;
	static addEnchant_t              addEnchant = reinterpret_cast<addEnchant_t>(Utils::FindSignature("48 89 5C 24 ?? 48 89 54 24 ?? 57 48 83 EC ?? 45 0F"));
	static saveEnchantsToUserData_t  saveEnchantsToUserData = reinterpret_cast<saveEnchantsToUserData_t>(Utils::FindSignature("40 57 48 83 EC ?? 48 C7 44 24 ?? FE FF FF FF 48 89 5C 24 ?? 48 8B FA 4C 8B C1 48 8B 01 48 85 C0"));

	if (getEnchantsFromUserData == 0x0) {
		uintptr_t sig = Utils::FindSignature("24 ?? 48 8D 95 ?? ?? ?? ?? 48 8D 8D ?? ?? ?? ?? E8 ?? ?? ?? ?? 90 45 33 C0 48") + 17;
		int offset = *reinterpret_cast<int*>(sig);
		getEnchantsFromUserData = reinterpret_cast<getEnchantsFromUserData_t>(sig + offset + 4);
	}
	if (strcmp(args->at(1).c_str(), "all") == 0)
	{
		for (int i = 0; i < 32; i++)
		{
			void* EnchantData = malloc(0x60);
			if (EnchantData != nullptr)
				memset(EnchantData, 0x0, 0x60);

			getEnchantsFromUserData(item, EnchantData);

			__int64 enchantPair = ((__int64)enchantLevel << 32) | i;

			if (addEnchant(EnchantData, enchantPair)) { // Upper 4 bytes = level, lower 4 bytes = enchant type
				saveEnchantsToUserData(item, EnchantData);
				__int64 proxy = reinterpret_cast<__int64>(g_Data.getLocalPlayer()->getSupplies());
				if (!*(uint8_t*)(proxy + 160))
					(*(void(__fastcall * *)(unsigned long long, unsigned long long, C_ItemStack*))(**(unsigned long long**)(proxy + 168) + 64i64))(
						*(unsigned long long*)(proxy + 168),
						*(unsigned int*)(proxy + 16),
						item);// Player::selectItem

				g_Data.getLocalPlayer()->sendInventory();
			}
			free(EnchantData);
		}
		clientMessageF("%sEnchant successful!", GREEN);
	}
	else
	{
		void* EnchantData = malloc(0x60);
		if (EnchantData != nullptr)
			memset(EnchantData, 0x0, 0x60);

		getEnchantsFromUserData(item, EnchantData);

		__int64 enchantPair = ((__int64)enchantLevel << 32) | enchantId;

		if (addEnchant(EnchantData, enchantPair)) { // Upper 4 bytes = level, lower 4 bytes = enchant type
			saveEnchantsToUserData(item, EnchantData);
			__int64 proxy = reinterpret_cast<__int64>(g_Data.getLocalPlayer()->getSupplies());
			if (!*(uint8_t*)(proxy + 160))
				(*(void(__fastcall * *)(unsigned long long, unsigned long long, C_ItemStack*))(**(unsigned long long**)(proxy + 168) + 64i64))(
					*(unsigned long long*)(proxy + 168),
					*(unsigned int*)(proxy + 16),
					item);// Player::selectItem

			g_Data.getLocalPlayer()->sendInventory();
			clientMessageF("%sEnchant successful!", GREEN);
		}
		else
			clientMessageF("%sEnchant failed, try using a lower enchant-level", RED);

		free(EnchantData);
	}
	return true;
}
