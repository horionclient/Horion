#include "EnchantCommand.h"
#include <map>

std::map<std::string, int> enchantMap;

EnchantCommand::EnchantCommand() : IMCCommand("enchant", "Enchants items", "<enchantment> [level]")
{
	enchantMap["Protection"] = 0;
	enchantMap["Fire_Protection"] = 1;
	enchantMap["Feather_Falling"] = 2;
	enchantMap["Blast_Protection"] = 3;
	enchantMap["Projectile_Protection"] = 4;
	enchantMap["Thorns"] = 5;
	enchantMap["Respiration"] = 6;
	enchantMap["Depth_Strider"] = 7;
	enchantMap["Aqua_Infinity"] = 8;
	enchantMap["Frost_walker"] = 25;
	enchantMap["Sharpness"] = 9;
	enchantMap["Smite"] = 10;
	enchantMap["Bane_of_Arthropods"] = 11;
	enchantMap["Knockback"] = 12;
	enchantMap["Fire_Aspect"] = 13;
	enchantMap["Looting"] = 14;
	enchantMap["Channeling"] = 32;
	enchantMap["Impaling"] = 29;
	enchantMap["Loyalty"] = 31;
	enchantMap["Riptide"] = 30;
	enchantMap["SilkTouch"] = 16;
	enchantMap["Fortune"] = 18;
	enchantMap["Unbreaking"] = 17;
	enchantMap["Efficiency"] = 15;
	enchantMap["Mending"] = 26;
	enchantMap["Power"] = 19;
	enchantMap["Punch"] = 20;
	enchantMap["Flame"] = 21;
	enchantMap["Infinity"] = 22;
	enchantMap["Multishot"] = 33;
	enchantMap["Quick_Charge"] = 35;
	enchantMap["Piercing"] = 34;
	enchantMap["Luck_of_Sea"] = 23;
	enchantMap["Lure"] = 24;
}


EnchantCommand::~EnchantCommand()
{
}

bool EnchantCommand::execute(std::vector<std::string>* args)
{
	assertTrue(args->size() > 1); // this line stops the command from been run if the proper args are not given

	int enchantId = 0;
	int enchantLevel = 32767;
	
	enchantId = enchantMap.find(args->at(1))->second;

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
