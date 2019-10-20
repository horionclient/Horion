#pragma once
#include "CEntity.h"

class C_Item;

class C_ItemStack {
public:
	C_Item** item;//0x0
private:
	char pad_0x008[0x12];//0x8
public:
	char count; //0x1A
private:
	char pad_0x1B[0x6D];//0x1B
public:
	C_ItemStack() = default;

	C_ItemStack(C_Item* item, int count, int itemData)
	{
		memset(this, 0x0, sizeof(C_ItemStack));
		using ItemStackContructor_t = void(__fastcall*)(C_ItemStack*, C_Item*, int, int);
		uintptr_t sigOffset = Utils::FindSignature("48 8B 13 ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 90 48 8B D0 ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 90") + 0xA;
		int offset = *reinterpret_cast<int*>((sigOffset + 1)); // Get Offset from code
		static ItemStackContructor_t  ItemStackContructor = reinterpret_cast<ItemStackContructor_t>(sigOffset + offset +5);
		if (ItemStackContructor != 0)
			ItemStackContructor(this, item, count, itemData);
	}

	C_ItemStack(C_BlockLegacy* legacy, int count)
	{
		memset(this, 0x0, sizeof(C_ItemStack));
		using ItemStackContructor_t = C_ItemStack * (__fastcall*)(C_ItemStack*, C_BlockLegacy*);
		static ItemStackContructor_t  ItemStackContructor = reinterpret_cast<ItemStackContructor_t>(Utils::FindSignature("40 53 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B D9 48 89 4C 24 28 33 C9 48 89 0B 48 89 4B 08 ?? ?? ?? ?? ?? ?? ?? 48 89 43 10 C7 43 ?? ?? ?? ?? ?? 48 89 4B 20 48 ?? ?? ?? ?? ?? ?? ?? C6 43 30 01 48 89 4B 38 48 89 4B 40 48 89 4B 48 48 89 4B 50 48 89 4B 58 48 89 4B 60 48 89 4B 68 48 89 4B 70 48 89 4B 78 ?? ?? ?? ?? ?? ?? ?? 44 8D 41 01 48 8B CB E8"));
		if (ItemStackContructor != 0)
			ItemStackContructor(this, legacy);
	}

	C_ItemStack(C_ItemStack const& src)
	{
		memset(this, 0x0, sizeof(C_ItemStack));
		using ItemStackCopyContructor_t = void(__fastcall*)(C_ItemStack&, C_ItemStack const&);
		static ItemStackCopyContructor_t  ItemStackCopyContructor = 0x0;
		if (ItemStackCopyContructor == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("E8 ?? ?? ?? ?? 90 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 90 F2 0F 10 44 24 ??");
			if (sigOffset != 0x0) {
				int offset = *reinterpret_cast<int*>((sigOffset + 1)); // Get Offset from code
				ItemStackCopyContructor = reinterpret_cast<ItemStackCopyContructor_t>(sigOffset + offset + /*length of instruction*/ 5);
			}
			if (ItemStackCopyContructor != 0)
				ItemStackCopyContructor(*this, src);
		}
	}

	/*C_ItemStack& operator=(C_ItemStack const& src)
	{
		using Destructor_t0 = void(__fastcall*)(C_ItemStack*);
		static Destructor_t0 Destruc = reinterpret_cast<Destructor_t0>(Utils::FindSignature("40 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 40 48 8B D9 ?? ?? ?? ?? ?? ?? ?? 48 85 FF 74 17 48 8B CF E8 ?? ?? ?? ?? 90 BA ?? ?? ?? ?? 48 8B CF E8 ?? ?? ?? ?? 90 48 8D 4B ?? E8 ?? ?? ?? ?? 90 48 8D 4B 38 E8 ?? ?? ?? ?? 90"));
		if (Destruc != 0)
			Destruc(this);
		memset(this, 0x0, sizeof(C_ItemStack));
		using ItemStackCopyContructor_t0 = void(__fastcall*)(C_ItemStack&, C_ItemStack const&);
		static ItemStackCopyContructor_t0  ItemStackCopyContruct = 0x0;
		if (ItemStackCopyContruct == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("E8 ?? ?? ?? ?? 90 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 90 F2 0F 10 44 24 ??");
			if (sigOffset != 0x0) {
				int offset = *reinterpret_cast<int*>((sigOffset + 1)); // Get Offset from code
				ItemStackCopyContruct = reinterpret_cast<ItemStackCopyContructor_t0>(sigOffset + offset + 5);
			}
			if (ItemStackCopyContruct != 0)
				ItemStackCopyContruct(*this, src);
		}
		return *this;
	}*/

	~C_ItemStack()
	{
		using Destructor_t = void(__fastcall*)(C_ItemStack*);
		static Destructor_t Destructor = reinterpret_cast<Destructor_t>(Utils::FindSignature("40 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 40 48 8B D9 ?? ?? ?? ?? ?? ?? ?? 48 85 FF 74 17 48 8B CF E8 ?? ?? ?? ?? 90 BA ?? ?? ?? ?? 48 8B CF E8 ?? ?? ?? ?? 90 48 8D 4B ?? E8 ?? ?? ?? ?? 90 48 8D 4B 38 E8 ?? ?? ?? ?? 90"));
		if (Destructor != 0)
			Destructor(this);
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

class C_Inventory {
private:
	virtual ~C_Inventory();
public:
	bool isFull()
	{
		int fullslots = 0;
		for (int i = 0; i < 36; i++)
		{
			if (this->getItemStack(i)->item != nullptr)
				fullslots++;
		}
		if (fullslots == 36) return true;
		return false;
	}
	int getFirstEmptySlot()
	{
		for (int i = 0; i < 36; i++)
		{
			if (this->getItemStack(i)->item == nullptr)
				return i;
		}
		return -1;
	}
	void dropSlot(int slot)
	{
		using drop_t = void(__fastcall*)(C_Inventory*, int, bool, bool, bool);
		static drop_t func = reinterpret_cast<drop_t>(Utils::FindSignature("85 D2 0F 88 ?? ?? ?? ?? 55 56 57 41 54 41 55 41 56 41 57 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? ?? 45 0F B6 D0 48 63 EA"));
		if (func != 0)
			func(this, slot,0,0,0);
	}
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
	void handleAutoPlace(uintptr_t a1, std::string name, int slot) {
		using ContainerScreenController__autoPlace = __int64(__fastcall*)(C_ContainerScreenController*, uintptr_t, TextHolder, int);
		static ContainerScreenController__autoPlace autoPlaceFunc = reinterpret_cast<ContainerScreenController__autoPlace>(Utils::FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 ?? ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? 45 8B F9 49 8B F8 44 8B EA 89 54 24 ?? 48 8B F1"));

		TextHolder txt = TextHolder(name);

		if (autoPlaceFunc != 0x0)
			autoPlaceFunc(this, a1, txt, slot);
	}
private:
	virtual __int64 destructor();
	virtual __int64 tick(void);
	virtual __int64 handleEvent(__int64&);
	virtual __int64 onOpen(void);
	virtual __int64 onTerminate(void);
	virtual __int64 onInit(void);
public:
	virtual bool canExit(void);
	virtual __int64 tryExit(void);
private:
	virtual __int64 areControllerTabsEnabled(void);
	virtual __int64 onCreation(void);
	virtual __int64 logCreationTime(std::string const&, double, double, unsigned char);
public:
	virtual __int64 onLeave(void);
	virtual __int64 leaveScreen(void);
private:
	virtual __int64 handleGameEventNotification(__int64);
	virtual __int64 bind(std::string const&, unsigned int, int, std::string const&, unsigned int, std::string const&, __int64&);
	virtual __int64 bind(std::string const&, unsigned int, std::string const&, __int64&);
	virtual __int64 handleLicenseChanged(void);
	virtual __int64 onDictationEvent(std::string const&);
public:
	virtual void setAssociatedvec3_ti(vec3_ti const&);
	virtual void setAssociatedEntityUniqueID(__int64);
	virtual void setSuspendInput(bool);
private:
	virtual __int64 getCallbackInterval(void)const;
public:
	virtual void setViewCommand(__int64);
private:
	virtual __int64 onRender(void);
	virtual __int64 addStaticScreenVars(__int64&);
	virtual __int64 getAdditionalScreenInfo(void)const;
	virtual __int64 getTelemetryOverride(void)const;
	virtual __int64 addEventProperties(__int64)const;
	virtual __int64 getSceneType(void)const;
	virtual __int64 getScreenVersion(void)const;
	virtual __int64 screenHandlesGamepadMenuButton(void)const;
	virtual __int64 getProxy(void);
	virtual __int64 onEntered(void);
	virtual __int64 getNameId(std::string const&)const;
	virtual __int64 _isStillValid(void)const;
	virtual __int64 _getGamepadHelperVisible(void)const;
	virtual __int64 _getMixedHelperVisible(void)const;
	virtual __int64 _getKeyboardHelperVisible(void)const;
	virtual __int64 _getButtonADescription(void);
	virtual __int64 _getButtonBDescription(void);
	virtual __int64 _getButtonXDescription(void);
	virtual __int64 _getButtonYDescription(void);
	virtual __int64 _getButtonKeyboardDescription(void);
public:
	virtual __int64 _handlePlaceAll(std::string const&, int);
	virtual __int64 _handlePlaceOne(std::string const&, int);
	virtual __int64 _handleSelectSlot(std::string const&, int);
	virtual __int64 _getSelectedItem(void)const;
	virtual C_ItemStack* _getItemStack(TextHolder, int)const;
private:
	virtual __int64 _getVisualItemStack(std::string const&, int)const;
	virtual __int64 _onContainerSlotHovered(std::string const&, int);
	virtual __int64 _onContainerSlotSelected(std::string const&, int);
	virtual __int64 _onContainerSlotPressed(std::string const&, int);
	virtual __int64 _shouldSwap(std::string const&, int, std::string const&, int)const;
	virtual __int64 _getCollectionName(__int64*)const;
	virtual __int64 _canSplit(std::string const&, int)const;
	virtual __int64 _sendFlyingItem(__int64 const&, std::string const&, int, std::string const&, int);
};

class C_CraftingScreenController : public C_ContainerScreenController
{

};

class C_ChestScreenController : public C_ContainerScreenController
{

};