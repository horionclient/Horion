#pragma once
#include "CEntity.h"

class C_Item;

class C_ItemStack {
private:
	uintptr_t** vTable;//0x0000
public:
	C_Item** item;   //0x08
private:
	char pad_0x008[0x12]; //0x10
public:
	char count; //0x22
private:
	char pad_0x1B[0x65];//0x23
public:
	C_ItemStack()
	{
		memset(this, 0x0, sizeof(C_ItemStack));
	}

	~C_ItemStack()
	{
		memset(this, 0x0, sizeof(C_ItemStack));
		using destructor_t = void(__fastcall*)(C_ItemStack*);
		static destructor_t func = reinterpret_cast<destructor_t>(0x0);
		if (!func)
		{
			uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 45 ?? 41 B2 01 80 7D C0 ?? 0F 84 ?? ?? ?? ?? 45 84 E4 0F 84");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** vTable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			func = reinterpret_cast<destructor_t>(vTable[0]);
		}
		func(this);
	}

	C_ItemStack(C_BlockLegacy& legacy, int count)
	{
		memset(this, 0x0, sizeof(C_ItemStack));
		using reinit_t = void(__fastcall*)(C_ItemStack*, C_BlockLegacy&, int);
		static reinit_t func = reinterpret_cast<reinit_t>(0x0);
		if (!func)
		{
			uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 45 ?? 41 B2 01 80 7D C0 ?? 0F 84 ?? ?? ?? ?? 45 84 E4 0F 84");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** vTable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			func = reinterpret_cast<reinit_t>(vTable[1]);
		}
		func(this, legacy, count);
	}

	C_ItemStack(C_Item& item, int count, int itemData)
	{
		memset(this, 0x0, sizeof(C_ItemStack));
		using reinitItem_t = void(__fastcall*)(C_ItemStack*, C_Item&, int,int);
		static reinitItem_t func = reinterpret_cast<reinitItem_t>(0x0);
		if (func == 0x0)
		{
			uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 45 ?? 41 B2 01 80 7D C0 ?? 0F 84 ?? ?? ?? ?? 45 84 E4 0F 84");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** vTable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			func = reinterpret_cast<reinitItem_t>(vTable[2]);
		}
		func(this,item, count, itemData);
	}

	C_ItemStack(C_ItemStack const& src)
	{
		memset(this, 0x0, sizeof(C_ItemStack));
		using ItemStackCopyConstructor_t = void(__fastcall*)(C_ItemStack&, C_ItemStack const&);
		static ItemStackCopyConstructor_t  ItemStackCopyConstructor = reinterpret_cast<ItemStackCopyConstructor_t>(Utils::FindSignature("48 8B C4 56 57 41 54 41 56 41 57 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 89 58 ?? 48 89 68 ?? 48 8B FA 48 8B D9 48 89 48 ??"));
		ItemStackCopyConstructor(*this, src);
		static uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 45 ?? 41 B2 01 80 7D C0 ?? 0F 84 ?? ?? ?? ?? 45 84 E4 0F 84");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		this->vTable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
	}
};

class C_Item {
private:
	char pad_0x0008[0x68];//0x8
public:
	uint16_t itemId;//0x70
private:
	char pad_0x00072[0x6];//0x72
public:
	TextHolder tileName;//0x78
	TextHolder name;//0x98
private:
	char pad_0x0B8[0x4C];//0xB8
public:
	int duration; //0x104
private:
	virtual __int64 destructor();
	virtual __int64 tearDown(void);
	virtual __int64 getMaxUseDuration(__int64 const*)const;
	virtual __int64 getMaxUseDuration(C_ItemStack const*)const;
public:
	virtual bool isExperimental(__int64 const*)const;
	virtual void setMaxStackSize(unsigned char);
	virtual void setCategory(int);
	virtual void setStackedByData(bool);
	virtual void setMaxDamage(int);
	virtual void setHandEquipped(void);
	virtual void setUseAnimation(int);
	virtual void setMaxUseDuration(int);
	virtual void setRequiresWorldBuilder(bool);
	virtual void setExplodable(bool);
	virtual void setIsGlint(bool);
	virtual void setShouldDespawn(bool);
private:
	virtual __int64 getC_BlockShape(void)const;
public:
	virtual bool canBeDepleted(void)const;
	virtual bool canDestroySpecial(C_Block const&)const;
private:
	virtual __int64 get__int64DataForAuxValue(int)const;
public:
	virtual bool isStackedByData(void)const;
public:
	virtual __int64 getMaxDamage(void)const;
	virtual int getAttackDamage(void)const;
public:
	virtual bool isHandEquipped(void)const;
	virtual bool isArmor(void)const;
	virtual bool isDye(void)const;
	virtual bool isFertilizer(int)const;
	virtual bool isGlint(__int64 const&)const;
	virtual bool isGlint(C_ItemStack const&)const;
	virtual bool isThrowable(void)const;
	virtual bool isPattern(void)const;
private:
	virtual __int64 showsDurabilityInCreative(void)const;
public:
	virtual bool isWearableThroughLootTable(C_ItemStack const&)const;
	virtual bool canDestroyInCreative(void)const;
	virtual bool isDestructive(int)const;
	virtual bool isLiquidClipItem(int)const;
private:
	virtual __int64 requiresInteract(void)const;
	virtual __int64 appendFormattedHovertext(C_ItemStack const&, __int64&, std::string&, bool)const;
	virtual __int64 appendFormattedHovertext(__int64 const&, __int64&, std::string&, bool)const;
public:
	virtual bool isValidRepairItem(C_ItemStack const&, C_ItemStack const&)const;
public:
	virtual __int64 getEnchantSlot(void)const;
	virtual __int64 getEnchantValue(void)const;
	virtual int getArmorValue(void)const;
public:
	virtual bool isComplex(void)const;
	virtual bool isValidAuxValue(int)const;
public:
	virtual __int64 getDamageChance(int)const;
	virtual __int64 uniqueAuxValues(void)const;
public:
	virtual bool isMultiColorTinted(C_ItemStack const&)const;
private:
	virtual __int64 getColor(__int64 const&)const;
	virtual __int64 getColor(C_ItemStack const&)const;
	virtual __int64 getBaseColor(C_ItemStack const&)const;
	virtual __int64 getSecondaryColor(C_ItemStack const&)const;
	virtual __int64 saveAdditionalData(__int64 const&, __int64&)const;
	virtual __int64 saveAdditionalData(C_ItemStack const&, __int64&)const;
	virtual __int64 readAdditionalData(C_ItemStack&, __int64 const&)const;
	virtual __int64 readAdditionalData(__int64&, __int64 const&)const;
public:
	virtual bool isTintable(void)const;
private:
	virtual __int64 use(C_ItemStack&, C_Entity&)const;
	virtual __int64 dispense(C_BlockSource&, __int64&, int, vec3_t const&, unsigned char)const;
	virtual __int64 useTimeDepleted(__int64&, __int64*, C_Entity*)const;
	virtual __int64 useTimeDepleted(C_ItemStack&, __int64*, C_Entity*)const;
	virtual __int64 releaseUsing(__int64&, C_Entity*, int)const;
	virtual __int64 releaseUsing(C_ItemStack&, C_Entity*, int)const;
	virtual __int64 getDestroySpeed(C_ItemStack const&, C_Block const&)const;
	virtual __int64 hurtEnemy(__int64&, __int64*, __int64*)const;
	virtual __int64 hurtEnemy(C_ItemStack&, __int64*, __int64*)const;
	virtual __int64 mineC_Block(__int64&, C_Block const&, int, int, int, C_Entity*)const;
	virtual __int64 mineC_Block(C_ItemStack&, C_Block const&, int, int, int, C_Entity*)const;
	virtual __int64 buildDescriptionId(__int64 const&)const;
	virtual __int64 buildDescriptionId(C_ItemStack const&)const;
	virtual __int64 buildEffectDescriptionName(__int64 const&)const;
	virtual __int64 buildEffectDescriptionName(C_ItemStack const&)const;
	virtual __int64 buildCategoryDescriptionName(__int64 const&)const;
	virtual __int64 buildCategoryDescriptionName(C_ItemStack const&)const;
	virtual __int64 readUserData(__int64&, __int64&, __int64&)const;
	virtual __int64 readUserData(C_ItemStack&, __int64&, __int64&)const;
	virtual __int64 writeUserData(__int64 const&, __int64&)const;
	virtual __int64 writeUserData(C_ItemStack const&, __int64&)const;
	virtual __int64 getMaxStackSize(__int64 const&)const;
	virtual __int64 inventoryTick(C_ItemStack&, __int64&, C_Entity&, int, bool)const;
	virtual __int64 refreshedInContainer(C_ItemStack&, __int64&)const;
	virtual __int64 refreshedInContainer(__int64&, __int64&)const;
	virtual __int64 getCooldownType(void)const;
	virtual __int64 getCooldownTime(void)const;
	virtual __int64 fixupOnLoad(__int64&)const;
	virtual __int64 fixupOnLoad(C_ItemStack&)const;
	virtual __int64 getDamageValue(C_ItemStack const&)const;
	virtual __int64 getDamageValue(__int64 const&)const;
public:
	virtual void setDamageValue(C_ItemStack&, short)const;
	virtual void setDamageValue(__int64&, short)const;
private:
	virtual __int64 getInHandUpdateType(C_Entity const&, __int64 const&, __int64 const&, bool, bool)const;
	virtual __int64 getInHandUpdateType(C_Entity const&, C_ItemStack const&, C_ItemStack const&, bool, bool)const;
public:
	virtual bool isSameItem(__int64 const&, __int64 const&)const;
	virtual bool isSameItem(C_ItemStack const&, C_ItemStack const&)const;
private:
	virtual __int64 getInteractText(C_Entity const&)const;
	virtual __int64 getAnimationFrameFor(__int64*, bool, C_ItemStack const*, bool)const;
public:
	virtual bool isEmissive(int)const;
private:
	virtual __int64 getIcon(int, int, bool)const;
	virtual __int64 getIconYOffset(void)const;
public:
	virtual void setIcon(std::string const&, int);
	virtual void setIcon(__int64 const&);
	virtual bool canBeCharged(void)const;
private:
	virtual __int64 playSoundIncrementally(__int64 const&, __int64&)const;
	virtual __int64 playSoundIncrementally(C_ItemStack const&, __int64&)const;
	virtual __int64 getAuxValuesDescription(void)const;
	virtual __int64 _checkUseOnPermissions(C_Entity&, __int64&, unsigned char const&, vec3_ti const&)const;
	virtual __int64 _checkUseOnPermissions(C_Entity&, C_ItemStack&, unsigned char const&, vec3_ti const&)const;
	virtual __int64 _calculatePlacePos(__int64&, C_Entity&, unsigned char&, vec3_ti&)const;
	virtual __int64 _calculatePlacePos(C_ItemStack&, C_Entity&, unsigned char&, vec3_ti&)const;
	virtual __int64 _useOn(__int64&, C_Entity&, vec3_ti, unsigned char, float, float, float)const;
	virtual __int64 _useOn(C_ItemStack&, C_Entity&, vec3_ti, unsigned char, float, float, float)const;
};

class C_ArmorItem : public C_Item
{
private:
	char pad_0x108[0x48];//0x108
public:
	int ArmorSlot; //0x150
};