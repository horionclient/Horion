#include "CEntity.h"

class C_Item;

class C_ItemStack {
public:
	C_Item** item;//0x0
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
	int duration;
private:
	virtual ~C_Item();
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
private:
	virtual __int64 getMaxDamage(void)const;
	virtual __int64 getAttackDamage(void)const;
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
	virtual __int64 appendFormattedHovertext(C_ItemStack const&, __int64 &, std::string &, bool)const;
	virtual __int64 appendFormattedHovertext(__int64 const&, __int64 &, std::string &, bool)const;
public:
	virtual bool isValidRepairItem(C_ItemStack const&, C_ItemStack const&)const;
private:
	virtual __int64 getEnchantSlot(void)const;
	virtual __int64 getEnchantValue(void)const;
	virtual __int64 getArmorValue(void)const;
public:
	virtual bool isComplex(void)const;
	virtual bool isValidAuxValue(int)const;
private:
	virtual __int64 getDamageChance(int)const;
	virtual __int64 uniqueAuxValues(void)const;
public:
	virtual bool isMultiColorTinted(C_ItemStack const&)const;
private:
	virtual __int64 getColor(__int64 const&)const;
	virtual __int64 getColor(C_ItemStack const&)const;
	virtual __int64 getBaseColor(C_ItemStack const&)const;
	virtual __int64 getSecondaryColor(C_ItemStack const&)const;
	virtual __int64 saveAdditionalData(__int64 const&, __int64 &)const;
	virtual __int64 saveAdditionalData(C_ItemStack const&, __int64 &)const;
	virtual __int64 readAdditionalData(C_ItemStack &, __int64 const&)const;
	virtual __int64 readAdditionalData(__int64 &, __int64 const&)const;
public:
	virtual bool isTintable(void)const;
private:
	virtual __int64 use(C_ItemStack &, C_Entity &)const;
	virtual __int64 dispense(C_BlockSource &, __int64 &, int, vec3_t const&, unsigned char)const;
	virtual __int64 useTimeDepleted(__int64 &, __int64 *, C_Entity *)const;
	virtual __int64 useTimeDepleted(C_ItemStack &, __int64 *, C_Entity *)const;
	virtual __int64 releaseUsing(__int64 &, C_Entity *, int)const;
	virtual __int64 releaseUsing(C_ItemStack &, C_Entity *, int)const;
	virtual __int64 getDestroySpeed(C_ItemStack const&, C_Block const&)const;
	virtual __int64 hurtEnemy(__int64 &, __int64 *, __int64 *)const;
	virtual __int64 hurtEnemy(C_ItemStack &, __int64 *, __int64 *)const;
	virtual __int64 mineC_Block(__int64 &, C_Block const&, int, int, int, C_Entity *)const;
	virtual __int64 mineC_Block(C_ItemStack &, C_Block const&, int, int, int, C_Entity *)const;
	virtual __int64 buildDescriptionId(__int64 const&)const;
	virtual __int64 buildDescriptionId(C_ItemStack const&)const;
	virtual __int64 buildEffectDescriptionName(__int64 const&)const;
	virtual __int64 buildEffectDescriptionName(C_ItemStack const&)const;
	virtual __int64 buildCategoryDescriptionName(__int64 const&)const;
	virtual __int64 buildCategoryDescriptionName(C_ItemStack const&)const;
	virtual __int64 readUserData(__int64 &, __int64 &, __int64 &)const;
	virtual __int64 readUserData(C_ItemStack &, __int64 &, __int64 &)const;
	virtual __int64 writeUserData(__int64 const&, __int64 &)const;
	virtual __int64 writeUserData(C_ItemStack const&, __int64 &)const;
	virtual __int64 getMaxStackSize(__int64 const&)const;
	virtual __int64 inventoryTick(C_ItemStack &, __int64 &, C_Entity &, int, bool)const;
	virtual __int64 refreshedInContainer(C_ItemStack &, __int64 &)const;
	virtual __int64 refreshedInContainer(__int64 &, __int64 &)const;
	virtual __int64 getCooldownType(void)const;
	virtual __int64 getCooldownTime(void)const;
	virtual __int64 fixupOnLoad(__int64 &)const;
	virtual __int64 fixupOnLoad(C_ItemStack &)const;
	virtual __int64 getDamageValue(C_ItemStack const&)const;
	virtual __int64 getDamageValue(__int64 const&)const;
public:
	virtual void setDamageValue(C_ItemStack &, short)const;
	virtual void setDamageValue(__int64 &, short)const;
private:
	virtual __int64 getInHandUpdateType(C_Entity const&, __int64 const&, __int64 const&, bool, bool)const;
	virtual __int64 getInHandUpdateType(C_Entity const&, C_ItemStack const&, C_ItemStack const&, bool, bool)const;
public:
	virtual bool isSameItem(__int64 const&, __int64 const&)const;
	virtual bool isSameItem(C_ItemStack const&, C_ItemStack const&)const;
private:
	virtual __int64 getInteractText(C_Entity const&)const;
	virtual __int64 getAnimationFrameFor(__int64 *, bool, C_ItemStack const*, bool)const;
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
	virtual __int64 playSoundIncrementally(__int64 const&, __int64 &)const;
	virtual __int64 playSoundIncrementally(C_ItemStack const&, __int64 &)const;
	virtual __int64 getAuxValuesDescription(void)const;
	virtual __int64 _checkUseOnPermissions(C_Entity &, __int64 &, unsigned char const&, vec3_ti const&)const;
	virtual __int64 _checkUseOnPermissions(C_Entity &, C_ItemStack &, unsigned char const&, vec3_ti const&)const;
	virtual __int64 _calculatePlacePos(__int64 &, C_Entity &, unsigned char &, vec3_ti &)const;
	virtual __int64 _calculatePlacePos(C_ItemStack &, C_Entity &, unsigned char &, vec3_ti &)const;
	virtual __int64 _useOn(__int64 &, C_Entity &, vec3_ti, unsigned char, float, float, float)const;
	virtual __int64 _useOn(C_ItemStack &, C_Entity &, vec3_ti, unsigned char, float, float, float)const;
};

class C_Inventory {
private:
	virtual ~C_Inventory();
public:
	virtual __int64 init();
	virtual __int64 addContentChangeListener(__int64 a2);
	virtual __int64 removeContentChangeListener(__int64 a2);
	virtual C_ItemStack* getItemStack(int slot);
	virtual __int64 addItem(C_ItemStack*);
	virtual __int64 addItemToFirstEmptySlot(C_ItemStack*);
};

class C_PlayerInventoryProxy {
private:
	char pad_0x0000[0x10]; //0x0000
public:
	int selectedHotbarSlot; //0x0010 
private:
	char pad_0x0014[0x8C]; //0x0014
public:
	uint8_t holdingItem; //0x00A0 
private:
	char pad_0x00A1[0x7]; //0x00A1
public:
	C_Inventory* inventory; //0x00A8 
};

class C_ContainerScreenController
{
public:
	void handleAutoPlace(uintptr_t idk, std::string name, int slot) {
		using ContainerScreenController__autoPlace = __int64(__fastcall*)(C_ContainerScreenController*, uintptr_t, TextHolder, int);
		static ContainerScreenController__autoPlace autoPlaceFunc = reinterpret_cast<ContainerScreenController__autoPlace>(Utils::FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 ?? ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? 45 8B F9 49 8B F8 44 8B EA 89 54 24 ?? 48 8B F1"));

		TextHolder txt = TextHolder(name);

		if (autoPlaceFunc != 0x0)
			autoPlaceFunc(this, idk, txt, slot);
	}
};

class C_ChestScreenController : public C_ContainerScreenController
{

};