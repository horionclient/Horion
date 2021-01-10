#pragma once
//#include "Tag.h"
#include <memory>
#include "TextHolder.h"
#include "../Utils/HMath.h"

class CompoundTag;
class Tag;
class C_BlockLegacy;
class C_BlockSource;
class C_Block;
class C_Entity;
class C_ItemStack;
class C_ScreenContext;
class C_ItemRenderer;
class C_ClientInstance;
class MinecraftGame;

class C_BaseActorRenderContext {
private:
	char pad_0x0[0x58];  //0x0000
public:
	C_ItemRenderer* renderer;  //0x0058
private:
	char pad_0x50[0x238];  //0x60
public:
	C_BaseActorRenderContext(C_ScreenContext* ScreenCtx, C_ClientInstance* client, MinecraftGame* game);
};

class C_ItemRenderer {
public:
	void renderGuiItemNew(C_BaseActorRenderContext* BaseActorRenderCtx, C_ItemStack* item, MinecraftGame* game, float x, float y, float opacity, float scale, bool isEnchanted);
	void renderGuiItemInChunk(C_BaseActorRenderContext* BaseActorRenderCtx, C_ItemStack* item, float x, float y, float opacity, float scale, bool isEnchanted, int mode);
};

class C_Item {
private:
	char pad_0x0008[0x62];  //0x8
public:
	uint16_t itemId;  //0x6A
private:
	char pad_0x00072[0x4];  //0x6C
public:
	TextHolder tileName;  //0x70
	TextHolder name;      //0x90
private:
	char pad_0x0B8[0x4C];  //0xB0
public:
	int duration;  //0xFC
private:
	virtual __int64 destructor();
	virtual __int64 tearDown(void);
	virtual __int64 getMaxUseDuration(__int64 const*) const;
	virtual __int64 getMaxUseDuration(C_ItemStack const*) const;

public:
	virtual bool isExperimental(__int64 const*) const;
	virtual bool returnFalse();
	virtual void setMaxStackSize(unsigned char);
	virtual void setCategory(int);
	virtual void setStackedByData(bool);
	virtual void setMaxDamage(int);
	virtual void setHandEquipped(void);
	virtual void setUseAnimation(int);
	virtual void setMaxUseDuration(int);
	virtual void setRequiresWorldBuilder(bool);
	virtual void setExplodable(bool);
	virtual void setFireResistant(bool);
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
	virtual bool requiresInteract();
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
	virtual __int64 getColor(C_ItemStack const&) const;
	virtual __int64 getBaseColor(C_ItemStack const&) const;
	virtual __int64 getSecondaryColor(C_ItemStack const&) const;
	virtual __int64 saveAdditionalData(C_ItemStack const&, __int64&) const;
	virtual __int64 readAdditionalData(C_ItemStack&, __int64 const&)const;

	virtual bool isTintable(void)const;
	virtual __int64 buildIdAux(short, const CompoundTag*);
	virtual __int64 buildDescriptor(short, const CompoundTag*);
	virtual __int64 use(C_ItemStack&, C_Entity&) const;
	virtual __int64 dispense(C_BlockSource&, __int64&, int, vec3_t const&, unsigned char) const;
	virtual __int64 useTimeDepleted(C_ItemStack&, __int64*, C_Entity*) const;
	virtual __int64 releaseUsing(C_ItemStack&, C_Entity*, int) const;
	virtual __int64 getDestroySpeed(C_ItemStack const&, C_Block const&) const;
	virtual __int64 hurtEnemy(__int64&, __int64*, __int64*) const;
	virtual __int64 hurtEnemy(C_ItemStack&, __int64*, __int64*) const;
	virtual __int64 mineC_Block(__int64&, C_Block const&, int, int, int, C_Entity*) const;
	virtual __int64 mineC_Block(C_ItemStack&, C_Block const&, int, int, int, C_Entity*) const;
	virtual __int64 buildDescriptionId(C_ItemStack const&) const;
	virtual __int64 buildEffectDescriptionName(C_ItemStack const&) const;
	virtual __int64 buildCategoryDescriptionName(C_ItemStack const&) const;
	virtual __int64 readUserData(C_ItemStack&, __int64&, __int64&) const;
	virtual __int64 writeUserData(C_ItemStack const&, __int64&) const;

public:
	virtual int getMaxStackSize(void* itemDescriptor) const;

private:
	virtual __int64 inventoryTick(C_ItemStack&, __int64&, C_Entity&, int, bool) const;
	//virtual __int64 refreshedInContainer(C_ItemStack&, __int64&)const;
	virtual __int64 refreshedInContainer(__int64&, __int64&) const;
	virtual __int64 getCooldownType(void) const;
	virtual __int64 getCooldownTime(void) const;
	virtual __int64 fixupOnLoad(__int64&) const;
	virtual __int64 fixupOnLoad(C_ItemStack&) const;
	//virtual __int64 getDamageValue(C_ItemStack const&) const;
	virtual __int64 getDamageValue(__int64 const&) const;

public:
	virtual void setDamageValue(C_ItemStack&, short) const;
	//virtual void setDamageValue(__int64&, short) const;

private:
	virtual __int64 getInHandUpdateType(C_Entity const&, __int64 const&, __int64 const&, bool, bool) const;
	virtual __int64 getInHandUpdateType(C_Entity const&, C_ItemStack const&, C_ItemStack const&, bool, bool) const;

public:
	//virtual bool isSameItem(__int64 const&, __int64 const&) const;
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
	virtual void setIconAtlas(__int64 const&);
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
		auto val = *reinterpret_cast<__int64***>(reinterpret_cast<__int64>(this) + 0x1D0);
		return val != nullptr && *val != nullptr;
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
	char pad_0x1B[0x6D];  //0x23
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

	C_ItemStack(C_ItemStack const& src);

	C_ItemStack(Tag const& tag);

	void fromTag(Tag const& tag);

	void save(CompoundTag** tag);

	void setUserData(std::unique_ptr<Tag> tag);

	bool isValid() {
		return this->item != nullptr && *this->item != nullptr;
	}

	inline C_Item* getItem() {
		return *this->item;
	}

	void reinit(C_BlockLegacy& legacy, int count);

	void reinit(C_Item& item, int count, int itemData);

	int getEnchantValue(int enchantId);

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

	void setShowPickUp(bool b) {
		*(bool*)(reinterpret_cast<__int64>(this)+ 0x30) = b;
	}

private:
	inline void setVtable(void);
};

class C_ArmorItem : public C_Item {
private:
	char pad_0x108[0xB8];  //0x100
public:
	int ArmorSlot;  //0x1B8

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


class ItemRegistry {
public:
	static C_Item*** getItemFromId(void* ptr, int itemId);
	static C_Item*** lookUpByName(void*, void*, TextHolder&);
};