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
	char pad_0x0[0x60];  //0x0000
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
	char pad_0008[0x70];     //0x0008
public:
	int16_t maxStackSize;   //0x0090
	int16_t itemId;         //0x0092
private:
	char pad_0094[4];       //0x0094
public:
	TextHolder tileName;    //0x0098
private:
	char pad_00B8[8];       //0x00B8
public:
	TextHolder name;        //0x00C0
private:
	char pad_00E0[8];       //0x00E0
public:
	TextHolder isFrom;		//0x00E8
private:
	char pad_0108[80];      //0x0108
public:
	int16_t duration;       //0x0158

/*
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
	int duration;  //0xFC*/
private:
	virtual __int64 destructor();                             // 0x0
	virtual __int64 initServer(int &);                // 0x1
	virtual __int64 tearDown(void);                           // 0x2
	virtual __int64 getMaxUseDuration(__int64 const *) const;  // 0x3
	virtual __int64 getMaxUseDuration(C_ItemStack const *);   // 0x4
public:
	virtual bool isMusicDisk(void);  // 0x5
private:
	virtual __int64 executeEvent(C_ItemStack &, std::string, char const &, int &);  // 0x6
	virtual __int64 isComponentBased(void);                                         // 0x7
public:
	virtual bool isArmor(void);             // 0x8
	virtual bool isBlockPlanterItem(void);  // 0x9
	virtual bool isBucket(void);            // 0xA
	virtual bool isCamera(void);            // 0xB
	virtual bool isCandle(void);            // 0xC
	virtual bool isDamageable(void);        // 0xD
	virtual bool isDyeable(void);           // 0xE
	virtual bool isDye(void);               // 0xF
private:
	virtual __int64 getItemColor(void);  // 0x10
public:
	virtual bool isFertilizer(void);  // 0x11
	virtual bool isFood(void);        // 0x12
	virtual bool isThrowable(void);   // 0x13
	virtual bool isUseable(void);     // 0x14
private:
	virtual __int64 getCamera(void);  // 0x15
	virtual __int64 getFood(void);    // 0x16
	virtual __int64 getFuel(void);    // 0x17
public:
	virtual __int64 setMaxStackSize(int);         // 0x18
	virtual __int64 setStackedByData(bool);         // 0x19
	virtual __int64 setMaxDamage(int);              // 0x1A
	virtual __int64 setHandEquipped(void);          // 0x1B
	virtual __int64 setUseAnimation(int);  // 0x1C
	virtual __int64 setMaxUseDuration(int);         // 0x1D
	virtual __int64 setRequiresWorldBuilder(bool);  // 0x1E
	virtual __int64 setExplodable(bool);            // 0x1F
	virtual __int64 setFireResistant(bool);         // 0x20
	virtual __int64 setIsGlint(bool);               // 0x21
	virtual __int64 setShouldDespawn(bool);         // 0x22
private:
	virtual __int64 getC_BlockShape(void) const;  // 0x23
public:
	virtual bool canBeDepleted(void) const;                 // 0x24
	virtual bool canDestroySpecial(C_Block const &) const;  // 0x25
private:
	virtual __int64 getLevelDataForAuxValue(int);  // 0x26
public:
	virtual bool isStackedByData(void);         // 0x27
	virtual __int64 getMaxDamage(void);         // 0x28
	virtual __int64 getAttackDamage(void);      // 0x29
	virtual bool isHandEquipped(void);          // 0x2A
	virtual bool isGlint(C_ItemStack const &);  // 0x2B
	virtual bool isPattern(void);               // 0x2C
private:
	virtual __int64 getPatternIndex(void);  // 0x2D
public:
	virtual bool showsDurabilityInCreative(void);                  // 0x2E
	virtual bool isWearableThroughLootTable(CompoundTag const *);  // 0x2F
	virtual bool canDestroyInCreative(void);                       // 0x30
	virtual bool isDestructive(int);                               // 0x31
private:
	virtual bool isLiquidClipItem(int);                                                                   // 0x32
	virtual bool shouldInteractionWithBlockBypassLiquid(C_Block const &);                                 // 0x33
	virtual bool requiresInteract(void);                                                                  // 0x34
	virtual __int64 appendFormattedHovertext(C_ItemStack const &, __int64 &, std::string &, bool) const;  // 0x35
public:
	virtual bool isValidRepairItem(C_ItemStack const &, C_ItemStack const &) const;  // 0x36
public:
	virtual __int64 getEnchantSlot(void);   // 0x37
	virtual __int64 getEnchantValue(void);  // 0x38
	virtual __int64 getArmorValue(void);    // 0x39
private:
	virtual bool isComplex(void);       // 0x3A
	virtual bool isValidAuxValue(int);  // 0x3B
public:
	virtual __int64 getDamageChance(int);   // 0x3C
	virtual __int64 getViewDamping(void);   // 0x3D
	virtual __int64 uniqueAuxValues(void);  // 0x3E
private:
	virtual __int64 isActorPlacerItem(void);  // 0x3F
public:
	virtual bool isMultiColorTinted(C_ItemStack const &) const;  // 0x40
private:
	virtual __int64 getColor(CompoundTag const *, C_ItemStack const &);                    // 0x41
	virtual __int64 hasCustomColor(C_ItemStack const &);                                   // 0x42
	virtual __int64 hasCustomColor(CompoundTag const *);                                   // 0x43
	virtual __int64 clearColor(C_ItemStack &);                                             // 0x44
	virtual __int64 clearColor(CompoundTag *);                                             // 0x45
	virtual __int64 setColor(C_ItemStack &, int const &);                           // 0x46
	virtual __int64 getBaseColor(C_ItemStack const &);                                     // 0x47
	virtual __int64 getSecondaryColor(C_ItemStack const &);                                // 0x48
	virtual __int64 getActorIdentifier(C_ItemStack const &);                               // 0x49
	virtual __int64 saveAdditionalData(C_ItemStack const &, CompoundTag &);                // 0x4A
	virtual __int64 readAdditionalData(C_ItemStack &, CompoundTag const &);                // 0x4B
	virtual __int64 buildIdAux(short, const CompoundTag *);                                // 0x4C
	virtual __int64 use(C_ItemStack &, C_Entity &);                                        // 0x4D
	virtual __int64 dispense(C_Block &, __int64 &, int, vec3_t const &, char);            // 0x4E
	virtual __int64 useTimeDepleted(C_ItemStack &, __int64 *, C_Entity *);                   // 0x4F
	virtual __int64 releaseUsing(C_ItemStack &, C_Entity *, int);                          // 0x50
	virtual __int64 getDestroySpeed(C_ItemStack const &, C_Block const &);                 // 0x51
	virtual __int64 hurtActor(C_ItemStack &, C_Entity &, C_Entity &);                      // 0x52
	virtual __int64 hitActor(C_ItemStack &, C_Entity &, C_Entity &);                       // 0x53
	virtual __int64 hitBlock(C_ItemStack &, C_Block const &, vec3_t const &, C_Entity &);  // 0x54
	virtual __int64 mineBlock(__int64 &&, C_Block const &, int, int, int, C_Entity *);     // 0x55
	virtual __int64 mineBlock(C_ItemStack &, C_Block const &, int, int, int, C_Entity *);  // 0x56
	virtual __int64 buildDescriptionName(C_ItemStack const &);                             // 0x57
	virtual __int64 buildDescriptionId(C_ItemStack const &) const;                         // 0x58
	virtual __int64 buildEffectDescriptionName(C_ItemStack const &);                       // 0x59
	virtual __int64 buildCategoryDescriptionName(void);                                    // 0x5A
	virtual __int64 readUserData(C_ItemStack &, __int64 &, __int64 &) const;               // 0x5B
	virtual __int64 writeUserData(C_ItemStack const &, __int64 &) const;                   // 0x5C
public:
	virtual int getMaxStackSize(void *itemDescriptor) const;  // 0x5D
private:
	virtual __int64 inventoryTick(C_ItemStack &, __int64 &, C_Entity &, int, bool) const;  // 0x5E
	virtual __int64 refreshedInContainer(C_ItemStack &, __int64 &) const;                  // 0x5F
	virtual __int64 getCooldownType(void);                                                 // 0x60
	virtual __int64 getCooldownTime(void);                                                 // 0x61
	virtual __int64 fixupCommon(C_ItemStack &);                                            // 0x62
	virtual __int64 fixupCommon(C_ItemStack &, __int64 &&);                                // 0x63
	virtual __int64 getDamageValue(C_ItemStack const &) const;                             // 0x64
public:
	virtual __int64 setDamageValue(C_ItemStack &, short);  // 0x65
private:
	virtual __int64 getInHandUpdateType(C_Entity const &, __int64 const &, __int64 const &, bool, bool) const;          // 0x66
	virtual __int64 getInHandUpdateType(C_Entity const &, C_ItemStack const &, C_ItemStack const &, bool, bool) const;  // 0x67
	virtual __int64 validFishInteraction(int);                                                                          // 0x68
public:
	virtual bool isSameItem(C_ItemStack const &, C_ItemStack const &) const;  // 0x69
private:
	virtual __int64 initClient(__int64 &);                                   // 0x6A
	virtual __int64 getInteractText(C_Entity const &);                                  // 0x6B
	virtual __int64 getAnimationFrameFor(C_Entity *, bool, C_ItemStack const *, bool);  // 0x6C
public:
	virtual bool isEmissive(int);           // 0x6D
	virtual __int64 getLightEmission(int);  // 0x6E
private:
	virtual __int64 getIcon(int, int, bool) const;  // 0x6F
	virtual __int64 getIconYOffset(void) const;     // 0x70
public:
	virtual void setIcon(std::string const &, int);  // 0x71
	virtual void setIcon(__int64 const &);           // 0x72
	virtual void setIconAtlas(__int64 const &);      // 0x73
	virtual bool canBeCharged(void) const;           // 0x74
private:
	virtual __int64 playSoundIncrementally(__int64 const &, __int64 &) const;      // 0x75
	virtual __int64 playSoundIncrementally(C_ItemStack const &, __int64 &) const;  // 0x76
	virtual __int64 getFurnaceBurnIntervalMultipler(C_ItemStack const &);          // 0x77
	virtual __int64 getFurnaceXPmultiplier(C_ItemStack const &);                   // 0x78
	virtual __int64 getAuxValuesDescription(void);                                 // 0x79
	virtual __int64 _checkUseOnPermissions(C_Entity &, C_ItemStack &, unsigned char const &, vec3_ti const &) const;	// 0x7A
	virtual __int64 _calculatePlacePos(C_ItemStack &, C_Entity &, unsigned char &, vec3_ti &) const;  // 0x7B

public:
	bool isTool(void) {
		if (getAttackDamage() > 0) return true;           // Does Attack Damage
		if (itemId == 300 || itemId == 301) return true;  // Bow
		if (itemId == 299) return true;                   // Flint n Steel
		return false;
	}
	bool isWeapon(void) {
		if (itemId == 308) return true;         //Wooden Sword
		if (itemId == 312) return true;         //Stone Sword
		if (itemId == 322) return true;         //Gold Sword
		if (itemId == 307) return true;         //Iron Sword
		if (itemId == 316) return true;         //Diamond Sword
		if (itemId == 602) return true;         //Netherite Sword
		if (itemId == 311) return true;         //Wooden Axe
		if (itemId == 315) return true;         //Stone Axe
		if (itemId == 325) return true;         //Gold Axe
		if (itemId == 298) return true;         //Iron Axe
		if (itemId == 319) return true;         //Diamond Axe
		if (itemId == 605) return true;         //Netherite Axe
		return false;
	}
	bool isBlock(void) {
		auto val = *reinterpret_cast<__int64***>(reinterpret_cast<__int64>(this) + 0x1A0);
		return val != nullptr && *val != nullptr;
	}
};


class ItemDescriptor {
private:
	char pad_0x0[0x48]; //0x0000
public:
	ItemDescriptor() {
		memset(this, 0x0, sizeof(ItemDescriptor));
	}
	ItemDescriptor(int id, int16_t itemData);
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
	char pad_0x108[0x228 - sizeof(C_Item)];  //0x100
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
