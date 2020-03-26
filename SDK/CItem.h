#pragma once
#include "Tag.h"
#include "CEntity.h"

class C_ItemStack;
class C_ScreenContext;
class C_ItemRenderer;
class C_ClientInstance;
class MinecraftGame;

class C_BaseActorRenderContext {
private:
	char pad_0x0[0x50];  //0x0000
public:
	C_ItemRenderer* renderer;  //0x0050
private:
	char pad_0x50[0x280 - 0x58];  //0x58
public:
	C_BaseActorRenderContext(C_ScreenContext* ScreenCtx, C_ClientInstance* client, MinecraftGame* game) {
		memset(this, 0, sizeof(C_BaseActorRenderContext));
		using BaseActorRenderContext_t = __int64(__fastcall*)(C_BaseActorRenderContext*, C_ScreenContext*, C_ClientInstance*, MinecraftGame*);
		static BaseActorRenderContext_t BaseActorRenderContext_constructor = reinterpret_cast<BaseActorRenderContext_t>(FindSignature("48 89 5C 24 08 57 48 83 EC 20 48 8D 05 ?? ?? ?? ?? 49 8B D8 48 89 01 48 8B F9 8B 42 ?? 89 41 ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 89 59 ?? 4C 89 49 ?? 48 89 51 ?? 49 8B C8"));
		BaseActorRenderContext_constructor(this, ScreenCtx, client, game);
	}
};

class C_ItemRenderer {
public:
	void renderGuiItemNew(C_BaseActorRenderContext* BaseActorRenderCtx, C_ItemStack* item, MinecraftGame* game, float x, float y, float opacity, float scale, bool isEnchanted) {
		using renderGuiItemNew_t = void(__fastcall*)(C_ItemRenderer*, C_BaseActorRenderContext*, C_ItemStack*, MinecraftGame*, float, float, float, float, float, bool);
		static renderGuiItemNew_t renderGuiItemNew = reinterpret_cast<renderGuiItemNew_t>(FindSignature("48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 89 58 20 0F 29 70 B8 48 8B 05 ?? ?? ?? ??"));
		renderGuiItemNew(this, BaseActorRenderCtx, item, game, x, y, 1, opacity, scale, isEnchanted);
	}
};

class C_Item {
private:
	char pad_0x0008[0x68];  //0x8
public:
	uint16_t itemId;  //0x70
private:
	char pad_0x00072[0x6];  //0x72
public:
	TextHolder tileName;  //0x78
	TextHolder name;      //0x98
private:
	char pad_0x0B8[0x4C];  //0xB8
public:
	int duration;  //0x104
private:
	virtual __int64 destructor();
	virtual __int64 tearDown(void);
	virtual __int64 getMaxUseDuration(__int64 const*) const;
	virtual __int64 getMaxUseDuration(C_ItemStack const*) const;

public:
	virtual bool isExperimental(__int64 const*) const;
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
	virtual __int64 getC_BlockShape(void) const;

public:
	virtual bool canBeDepleted(void) const;
	virtual bool canDestroySpecial(C_Block const&) const;

private:
	virtual __int64 get__int64DataForAuxValue(int) const;

public:
	virtual bool isStackedByData(void) const;

public:
	virtual __int64 getMaxDamage(void) const;
	virtual int getAttackDamage(void) const;

public:
	virtual bool isHandEquipped(void) const;
	virtual bool isArmor(void) const;
	virtual bool isDye(void) const;
	virtual bool isFertilizer(int) const;
	virtual bool isGlint(__int64 const&) const;
	virtual bool isGlint(C_ItemStack const&) const;
	virtual bool isThrowable(void) const;
	virtual bool isPattern(void) const;

private:
	virtual __int64 showsDurabilityInCreative(void) const;

public:
	virtual bool isWearableThroughLootTable(C_ItemStack const&) const;
	virtual bool canDestroyInCreative(void) const;
	virtual bool isDestructive(int) const;
	virtual bool isLiquidClipItem(int) const;

private:
	virtual __int64 appendFormattedHovertext(C_ItemStack const&, __int64&, std::string&, bool) const;

public:
	virtual bool isValidRepairItem(C_ItemStack const&, C_ItemStack const&) const;

public:
	virtual __int64 getEnchantSlot(void) const;
	virtual __int64 getEnchantValue(void) const;
	virtual int getArmorValue(void) const;

public:
	virtual bool isComplex(void) const;
	virtual bool isValidAuxValue(int) const;

public:
	virtual __int64 getDamageChance(int) const;
	virtual __int64 uniqueAuxValues(void) const;

public:
	virtual bool isMultiColorTinted(C_ItemStack const&) const;

private:
	virtual __int64 getColor(__int64 const&) const;
	virtual __int64 getColor(C_ItemStack const&) const;
	virtual __int64 getBaseColor(C_ItemStack const&) const;
	virtual __int64 getSecondaryColor(C_ItemStack const&) const;
	virtual __int64 saveAdditionalData(__int64 const&, __int64&) const;
	virtual __int64 saveAdditionalData(C_ItemStack const&, __int64&) const;
	/*	virtual __int64 readAdditionalData(C_ItemStack&, __int64 const&)const;
		virtual __int64 readAdditionalData(__int64&, __int64 const&)const;
	public:
		virtual bool isTintable(void)const;
	private:*/
	virtual __int64 use(C_ItemStack&, C_Entity&) const;
	virtual __int64 dispense(C_BlockSource&, __int64&, int, vec3_t const&, unsigned char) const;
	virtual __int64 useTimeDepleted(__int64&, __int64*, C_Entity*) const;
	virtual __int64 useTimeDepleted(C_ItemStack&, __int64*, C_Entity*) const;
	virtual __int64 releaseUsing(__int64&, C_Entity*, int) const;
	virtual __int64 releaseUsing(C_ItemStack&, C_Entity*, int) const;
	virtual __int64 getDestroySpeed(C_ItemStack const&, C_Block const&) const;
	virtual __int64 hurtEnemy(__int64&, __int64*, __int64*) const;
	virtual __int64 hurtEnemy(C_ItemStack&, __int64*, __int64*) const;
	virtual __int64 mineC_Block(__int64&, C_Block const&, int, int, int, C_Entity*) const;
	virtual __int64 mineC_Block(C_ItemStack&, C_Block const&, int, int, int, C_Entity*) const;
	//virtual __int64 buildDescriptionId(__int64 const&)const;
	virtual __int64 buildDescriptionId(C_ItemStack const&) const;
	//virtual __int64 buildEffectDescriptionName(__int64 const&)const;
	virtual __int64 buildEffectDescriptionName(C_ItemStack const&) const;
	//virtual __int64 buildCategoryDescriptionName(__int64 const&)const;
	virtual __int64 buildCategoryDescriptionName(C_ItemStack const&) const;
	virtual __int64 readUserData(__int64&, __int64&, __int64&) const;
	virtual __int64 readUserData(C_ItemStack&, __int64&, __int64&) const;
	//virtual __int64 writeUserData(__int64 const&, __int64&)const;
	virtual __int64 writeUserData(C_ItemStack const&, __int64&) const;

public:
	virtual int getMaxStackSize(void) const;

private:
	virtual __int64 inventoryTick(C_ItemStack&, __int64&, C_Entity&, int, bool) const;
	//virtual __int64 refreshedInContainer(C_ItemStack&, __int64&)const;
	virtual __int64 refreshedInContainer(__int64&, __int64&) const;
	virtual __int64 getCooldownType(void) const;
	virtual __int64 getCooldownTime(void) const;
	virtual __int64 fixupOnLoad(__int64&) const;
	virtual __int64 fixupOnLoad(C_ItemStack&) const;
	virtual __int64 getDamageValue(C_ItemStack const&) const;
	virtual __int64 getDamageValue(__int64 const&) const;

public:
	virtual void setDamageValue(C_ItemStack&, short) const;
	virtual void setDamageValue(__int64&, short) const;

private:
	virtual __int64 getInHandUpdateType(C_Entity const&, __int64 const&, __int64 const&, bool, bool) const;
	virtual __int64 getInHandUpdateType(C_Entity const&, C_ItemStack const&, C_ItemStack const&, bool, bool) const;

public:
	virtual bool isSameItem(__int64 const&, __int64 const&) const;
	virtual bool isSameItem(C_ItemStack const&, C_ItemStack const&) const;

private:
	virtual __int64 getInteractText(C_Entity const&) const;
	virtual __int64 getAnimationFrameFor(__int64*, bool, C_ItemStack const*, bool) const;

public:
	virtual bool isEmissive(int) const;

private:
	virtual __int64 getIcon(int, int, bool) const;
	virtual __int64 getIconYOffset(void) const;

public:
	virtual void setIcon(std::string const&, int);
	virtual void setIcon(__int64 const&);
	virtual bool canBeCharged(void) const;

private:
	virtual __int64 playSoundIncrementally(__int64 const&, __int64&) const;
	virtual __int64 playSoundIncrementally(C_ItemStack const&, __int64&) const;
	virtual __int64 getAuxValuesDescription(void) const;
	virtual __int64 _checkUseOnPermissions(C_Entity&, __int64&, unsigned char const&, vec3_ti const&) const;
	virtual __int64 _checkUseOnPermissions(C_Entity&, C_ItemStack&, unsigned char const&, vec3_ti const&) const;
	virtual __int64 _calculatePlacePos(__int64&, C_Entity&, unsigned char&, vec3_ti&) const;
	virtual __int64 _calculatePlacePos(C_ItemStack&, C_Entity&, unsigned char&, vec3_ti&) const;
	virtual __int64 _useOn(__int64&, C_Entity&, vec3_ti, unsigned char, float, float, float) const;
	virtual __int64 _useOn(C_ItemStack&, C_Entity&, vec3_ti, unsigned char, float, float, float) const;

public:
	bool isTool(void) {
		if (getAttackDamage() > 0) return true;           // Does Attack Damage
		if (itemId == 261 || itemId == 262) return true;  // Bow
		if (itemId == 259) return true;                   // Flint n Steel
		return false;
	}
	bool isFood(void) {
		if (itemId == 322 || itemId == 466) return true;                                                                      // Golden Apple
		if (itemId == 260) return true;                                                                                       // Apple
		if (itemId == 282) return true;                                                                                       // Mushroom Stew
		if (itemId == 297) return true;                                                                                       // Bread
		if (itemId == 319 || itemId == 320) return true;                                                                      // Porkchop
		if (itemId == 349 || itemId == 350 || itemId == 460 || itemId == 461 || itemId == 462 || itemId == 463) return true;  // Fish
		if (itemId == 357) return true;                                                                                       // Cookie
		if (itemId == 360) return true;                                                                                       // Melon
		if (itemId == 363 || itemId == 364) return true;                                                                      // Beef
		if (itemId == 365 || itemId == 366) return true;                                                                      // Chicken
		if (itemId == 391) return true;                                                                                       // Carrot
		if (itemId == 392 || itemId == 393) return true;                                                                      // Potato
		if (itemId == 400) return true;                                                                                       // Pumpkin Pie
		if (itemId == 411 || itemId == 412 || itemId == 413) return true;                                                     // Rabbit thing
		if (itemId == 423 || itemId == 424) return true;                                                                      // Mutton
		if (itemId == 450) return true;
		return false;
	}
	bool isBlock(void) {
		if (itemId != 0 && itemId < 255) return true;
		return false;
	}
};

class C_ItemStack {
private:
	uintptr_t** vTable;  //0x0000
public:
	C_Item** item;  //0x08
	CompoundTag* tag;  //0x10
private:
	char pad_0x18[0xA]; //0x18
public:
	char count;  //0x22
private:
	char pad_0x1B[0x65];  //0x23
public:
	C_ItemStack() {
		memset(this, 0x0, sizeof(C_ItemStack));
	}

	C_ItemStack(C_BlockLegacy& legacy, int count) {
		memset(this, 0x0, sizeof(C_ItemStack));
		reinit(legacy, count);
	}

	C_ItemStack(C_Item& item, int count, int itemData) {
		memset(this, 0x0, sizeof(C_ItemStack));
		reinit(item, count, itemData);
	}

	C_ItemStack(C_ItemStack const& src) {
		memset(this, 0x0, sizeof(C_ItemStack));
		using ItemStackCopyConstructor_t = void(__fastcall*)(C_ItemStack&, C_ItemStack const&);
		static ItemStackCopyConstructor_t ItemStackCopyConstructor = reinterpret_cast<ItemStackCopyConstructor_t>(FindSignature("48 8B C4 56 57 41 54 41 56 41 57 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 89 58 ?? 48 89 68 ?? 48 8B FA 48 8B D9 48 89 48 ??"));
		ItemStackCopyConstructor(*this, src);
		this->setVtable();
	}

	C_ItemStack(Tag const& tag) {
		memset(this, 0x0, sizeof(C_ItemStack));
		this->setVtable();
		using ItemStackBase__loadItemF = void(__fastcall*)(C_ItemStack*,Tag const&);
		static ItemStackBase__loadItemF ItemStackBase__loadItem = reinterpret_cast<ItemStackBase__loadItemF>(FindSignature("48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 C7 45 ? ? ? ? ? 48 89 58 18 0F 29 70 B8 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA 4C 8B E9"));
		ItemStackBase__loadItem(this, tag);
	}

	void fromTag(Tag const& tag) {
		using ItemStackBase__loadItemF = void(__fastcall*)(C_ItemStack*,Tag const&);
		static ItemStackBase__loadItemF fromTag = reinterpret_cast<ItemStackBase__loadItemF>(FindSignature("48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 C7 45 ? ? ? ? ? 48 89 58 18 0F 29 70 B8 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA 4C 8B E9"));
		fromTag(this, tag);
	}

	void save(CompoundTag** tag) {
		using ItemStackBase__saveF = void(__fastcall*)(C_ItemStack*,CompoundTag**);
		ItemStackBase__saveF save = reinterpret_cast<ItemStackBase__saveF>(FindSignature("40 55 56 57 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 C7 45 ? ? ? ? ? 48 89 9C 24 ? ? ? ? 48 8B F2 4C 8B F1 48 89 55"));
		return save(this,tag);
	}

	void setUserData(std::unique_ptr<Tag> tag) {
		using setUserData_t = void(__fastcall*)(C_ItemStack*, std::unique_ptr<Tag>);
		setUserData_t setUserData = reinterpret_cast<setUserData_t>(FindSignature("40 53 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B DA 48 8D 51 10 48 3B D3 74 20 48 8B 03 48 ?? ?? ?? ?? ?? ?? 48 8B 0A 48 89 02 48 85 C9 74 0B 48 8B 01 BA ?? ?? ?? ?? FF 10"));
		setUserData(this, std::move(tag));
	}

	bool isValid() {
		return this != nullptr && this->item != nullptr && *this->item != nullptr;
	}

	inline C_Item* getItem() {
		return *this->item;
	}

	void reinit(C_BlockLegacy& legacy, int count) {
		this->setVtable();
		Utils::CallVFunc<1, void>(this, &legacy, count);
	}

	void reinit(C_Item& item, int count, int itemData) {
		this->setVtable();
		Utils::CallVFunc<2, void>(this, &item, count, itemData);
	}

	int getEnchantValue(int enchantId) {
		using getEnchantsLevel_t = int(__fastcall*)(int, C_ItemStack*);
		static getEnchantsLevel_t getEnchantsLevel = reinterpret_cast<getEnchantsLevel_t>(FindSignature("48 8B C4 57 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 89 58 ?? 48 89 70 ?? 48 8B F2 8B ?? 33 ?? 48 8B"));
		return getEnchantsLevel(enchantId, this);
	}

	bool isEnchanted() {
		int enchantValue = 0;
		for (int i = 0; i < 40; i++) {
			enchantValue = this->getEnchantValue(i);
		}
		if (enchantValue != 0)
			return true;
		else
			return false;
	}

	float getArmorValueWithEnchants() {
		if (!this->item || !(*this->item)->isArmor())
			return 0;

		return (float)(((*this->item)->getArmorValue() + ((this->getEnchantValue(0) * 1.5f		// Protection
														   + this->getEnchantValue(5) * 0.4f	// Thorns
														   + this->getEnchantValue(3) * 0.4f	// Blast Protection
														   + this->getEnchantValue(1) * 0.4f	// Fire Protection
														   + this->getEnchantValue(4) * 0.4f	// Projectile Protection
			))));
	}

	float getAttackingDamageWithEnchants() {
		if (!this->item)
			return 0;
		int sharpnessValue = this->getEnchantValue(9);
		return (*this->item)->getAttackDamage() + 1.25f * sharpnessValue;
	}

private:
	inline void setVtable(void) {
		static uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 7C 24 ?? 00 75 31");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		this->vTable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
	}
};

class C_ArmorItem : public C_Item {
private:
	char pad_0x108[0x48 + 0x70];  //0x108
public:
	int ArmorSlot;  //0x150

	bool isHelmet() {
		return ArmorSlot == 0;
	}
	bool isChestplate() {
		return ArmorSlot == 1;
	}
	bool isLeggings() {
		return ArmorSlot == 2;
	}
	bool isBoots() {
		return ArmorSlot == 3;
	}
};
