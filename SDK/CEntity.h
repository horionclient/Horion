#pragma once

#include "../Utils/HMath.h"
#include "CBlockLegacy.h"
#include "CEntityList.h"
#include "CInventory.h"
#include "CInventoryTransaction.h"
#include "TextHolder.h"

class C_GameMode;

class PointingStruct {
private:
	char pad_0x0000[0x8E0];  //0x0000
public:
	int rayHitType;       //0x08E0
	int blockSide;        //0x08E4
	vec3_ti block;        //0x08E8
	vec3_t rayHitVec;     //0x08F4
	C_Entity *entityPtr;  //0x0900
};

class C_Player;

#pragma pack(push, 4)
class C_Entity {
public:
	uintptr_t *ptrToPtrToEntList;  //0x0008
private:
	char pad_0010[0x110];  //0x0010
public:
	union {
		struct {
			float pitch;  //0x0120
			float yaw;    //0x0124
		};
		vec2_t viewAngles;
	};
	union {
		struct {
			float pitch2;  
			float yaw2;    
		};
		vec2_t viewAngles2; // 0x128
	};

private:
	char pad_0100[0x10];  //0x0130
public:
	vec3_t eyePos0;  //0x0140
private:
	char pad_0x11C[0x70];  //0x14C
public:
	float fallDistance; //0x1BC
	bool onGround;  //0x01C0
private:
	char pad_0x179[0x5F];  //0x1C1
public:
	float stepHeight; //0x220
private:
	char pad_0x228[0x10];  //0x224
public:
	float web;  //0x0234
private:
	char pad_0x23C[0xD];  //0x0238
public:
	bool didEnterWaterBool;  //0x0245
private:
	char pad_023E[0x4E];  //0x0246
public:
	int ticksAlive;  //0x0294
private:
	char pad_0290[0xB0];  //0x0298
public:
	C_BlockSource *region;  //0x348
private:
	char pad_0x340[0x148];  //0x350
public:
	AABB aabb;  //0x0498
	float width;        //0x04B4
	float height;       //0x04B8
	vec3_t currentPos;  //0x04BC
	vec3_t oldPos;      //0x04C8
	vec3_t velocity;    //0x04D4
private:
	char pad_0x0478[0x58];  //0x4E0
public:
	__int64 entityRuntimeId;  //0x538
private:
	char pad_0478[0x148];  //0x0540
public:
	float bodyYaw;     //0x0688
	float oldBodyYaw;  //0x068C
	float yawUnused1;  //0x0690
	float yawUnused2;  //0x0694
	int damageTime;    //0x0698
private:
	char pad_0824[0x8C];  //0x069C
public:
	int timeSinceDeath;  //0x0728
private:
	char pad_08B0[0x200];  //0x072C
public:
	bool canFly;  //0x092C
private:
	char pad_0x0A95[0x6C3];  //0x092D
public:
	int16_t itemData;        //0x0FF0
	int16_t itemId;          //0x0FF2
private:
	char pad_10CC[0x1FC];  //0x0FF4
public:
	C_InventoryTransactionManager transac;  //0x11F0
private:
	char pad_0x1324[0xAA4];  //0x1258
public:
	int gamemode;  //0x1CFC
private:
	char pad_1DA4[0x260];  //0x1D00
public:
	TextHolder uuid;  //0x1F60

public:
	virtual bool hasComponent(__int64 const &) const;

private:
	virtual __int64 reloadHardcoded(__int64, __int64 const &);
	virtual __int64 reloadHardcodedClient(__int64, __int64 const &);
	virtual __int64 initializeComponents(__int64, __int64 const &);
	virtual __int64 reloadComponents(__int64, __int64 const &);
	virtual __int64 _serverInitItemStackIds();
	virtual __int64 _doInitialMove();

private:
	virtual void destructor();
	// Duplicate destructor
	virtual __int64 reset(void);
	virtual __int64 getOnDeathExperience(void);
	virtual __int64 getOwnerEntityType(void);
	virtual __int64 remove(void);

public:
	virtual void setPos(vec3_t const &);
	virtual vec3_t *getPredictedMovementValues(void) const;
	virtual vec3_t *getPos(void) const;
	virtual vec3_t *getPosOld(void) const;
	virtual vec3_t *getPosExtrapolated(float) const;

private:
	virtual __int64 getAttachPos(__int64, float) const;
	virtual __int64 getFiringPos(void) const;

public:
	virtual void setRot(vec2_t const &);
	virtual void move(class IActorMovementProxy &, vec3_t const &);
	virtual void move(vec3_t const &);
	virtual __int64 getInterpolatedRidingPosition(float) const;
	virtual __int64 getInterpolatedBodyRot(float) const;
	virtual __int64 getInterpolatedHeadRot(float) const;
	virtual __int64 getInterpolatedBodyYaw(float) const;
	virtual __int64 getYawSpeedInDegreesPerSecond(void) const;
	virtual __int64 getInterpolatedWalkAnimSpeed(float) const;
	virtual __int64 getWorldPosition();
	virtual void checkBlockCollisions(AABB const &);
	virtual void checkBlockCollisions(void);
	virtual bool isFireImmune();
	virtual bool breaksFallingBlocks(void) const;
	virtual bool blockedByShield(__int64 const &, C_Entity &);
	virtual void teleportTo(vec3_t const &, bool, int, int);
	virtual void tryTeleportTo(vec3_t const &, bool, bool, int, int);
	virtual void chorusFruitTeleport(vec3_t &);
	virtual __int64 lerpTo(vec3_t const &, vec2_t const &, int);
	virtual void lerpMotion(vec3_t const &);

private:
	virtual __int64 getAddPacket(void);
	virtual __int64 normalTick(void);
	virtual __int64 baseTick(void);
	virtual __int64 rideTick(void);
	virtual __int64 positionRider(C_Entity &, float);
	virtual __int64 getRidingHeight(void);
	virtual __int64 startRiding(C_Entity &);
	virtual __int64 addRider(C_Entity &);
	virtual __int64 flagRiderToRemove(C_Entity &);
	virtual __int64 getExitTip(std::string const &, __int64) const;
	virtual __int64 intersects(vec3_t const &, vec3_t const &) const;

public:
	virtual bool isFree(vec3_t const &, float);
	virtual bool isFree(vec3_t const &);
	virtual bool isInWall(void) const;
	virtual bool isInvisible(void) const;
	virtual bool canShowNameTag(void) const;
	virtual bool canExistInPeaceful(void) const;
	virtual void setNameTagVisible(bool);
	virtual TextHolder *getNameTag(void) const;

private:
	virtual __int64 getNameTagAsHash(void) const;

public:
	virtual TextHolder *getFormattedNameTag(void) const;

private:
	virtual __int64 filterFormattedNameTag(__int64 const &);

public:
	virtual void setNameTag(std::string const &);

private:
	virtual __int64 getAlwaysShowNameTag(void) const;

public:
	virtual void setScoreTag(std::string const &);
	virtual TextHolder *getScoreTag(void) const;

public:
	virtual bool isInWater(void) const;
	virtual bool hasEnteredWater(void) const;
	virtual bool isImmersedInWater(void) const;
	virtual bool isInWaterOrRain(void) const;
	virtual bool isInLava(void) const;
	virtual bool isUnderLiquid(__int64) const;
	virtual bool isOverWater(void) const;

private:
	virtual __int64 makeStuckInBlock(float);
	virtual __int64 getCameraOffset(void) const;
	virtual __int64 getShadowHeightOffs(void);
	virtual __int64 getShadowRadius(void) const;
	virtual __int64 getHeadLookVector(float);

public:
	virtual bool canSeeInvisible(void) const;
	virtual bool canSee(vec3_t const &) const;
	virtual bool canSee(C_Entity const &) const;
	virtual bool isSkyLit(float);

private:
	virtual __int64 getBrightness(float) const;
	virtual __int64 interactPreventDefault(void);
	virtual __int64 playerTouch(C_Player &);
	virtual __int64 onAboveBubbleColumn(bool);
	virtual __int64 onInsideBubbleColumn(bool);

public:
	virtual bool isImmobile(void) const;
	virtual bool isSilent(void);
	virtual bool isPickable(void);
	virtual bool isFishable(void) const;
	virtual bool isSleeping();
	virtual bool isPushable(void) const;
	virtual void setSneaking(bool);
	virtual bool isBlocking(void) const;
	virtual bool isDamageBlocked(__int64 const &) const;
	virtual bool isAlive(void) const;
	virtual bool isOnFire(void) const;
	virtual bool isOnHotBlock(void) const;
	virtual bool isCreativeModeAllowed(void);
	virtual bool isSurfaceMob(void) const;
	virtual bool isTargetable(void) const;

private:
	virtual bool unk1(void) const;
	virtual bool unk2(void) const;

public:
	virtual bool canAttack(C_Entity *, bool) const;
	virtual void setTarget(C_Entity *);

private:
	virtual __int64 findAttackTarget(void);

public:
	virtual bool isValidTarget(C_Entity *) const;

private:
	virtual __int64 attack(C_Entity &);
	virtual void performRangedAttack(C_Entity &, float);
	virtual __int64 adjustDamageAmount(int &) const;
	virtual __int64 getEquipmentCount(void) const;

public:
	virtual void setOwner(__int64);
	virtual void setSitting(bool);

private:
	virtual __int64 onTame(void);
	virtual __int64 onFailedTame(void);
	virtual __int64 getInventorySize(void) const;
	virtual __int64 getEquipSlots(void) const;
	virtual __int64 getChestSlots(void) const;

public:
	virtual void setStanding(bool);
	virtual bool canPowerJump(void) const;
	virtual void setCanPowerJump(bool);
	virtual bool isJumping(void) const;
	virtual bool isEnchanted(void) const;

private:
	virtual __int64 rideJumped(void);
	virtual __int64 rideLanded(vec3_t const &, vec3_t const &);
	virtual __int64 shouldRender(void) const;

public:
	virtual bool isInvulnerableTo(__int64 const &) const;

private:
	virtual __int64 getBlockDamageCause();
	virtual void actuallyHurt(int, __int64 const *, bool);
	virtual __int64 animateHurt(void);
	virtual __int64 doFireHurt(int);
	virtual __int64 onLightningHit(void);
	virtual __int64 onBounceStarted(vec3_ti const &, __int64 const &);
	virtual __int64 feed(int);
	virtual __int64 handleEntityEvent(__int64, int);
	virtual __int64 getPickRadius(void);
	virtual const class HashedString* getActorRendererId(void);
	virtual __int64 spawnAtLocation(int, int);
	virtual __int64 spawnAtLocation(int, int, float);
	virtual __int64 spawnAtLocation(__int64 const &, int);
	virtual __int64 spawnAtLocation(__int64 const &, int, float);
	virtual __int64 spawnAtLocation(__int64 const &, float);
	virtual __int64 despawn(void);
	virtual __int64 killed(C_Entity &);
	virtual __int64 awardKillScore(C_Entity &, int);

public:
	virtual void setArmor(int, C_ItemStack const &);
	virtual C_ItemStack *getArmor(int) const;

private:
	virtual __int64 getArmorMaterialTypeInSlot(__int64) const;
	virtual __int64 getArmorMaterialTextureTypeInSlot(__int64) const;
	virtual __int64 getArmorColorInSlot(__int64, int) const;

public:
	virtual void getEquippedSlot(__int64);
	virtual void setEquippedSlot(__int64, __int64 const &);

private:
	virtual __int64 getCarriedItem(void) const;

public:
	virtual void setCarriedItem(__int64 const &);
	virtual void setOffhandSlot(C_ItemStack *);
	virtual C_ItemStack *getEquippedTotem(void) const;
	virtual __int64 consumeTotem(void);

public:
	virtual __int64 save(CompoundTag *);
	virtual __int64 saveWithoutId(CompoundTag *);

private:
	virtual __int64 load(__int64 const &, __int64 &);
	virtual __int64 loadLinks(__int64 const &, __int64);

public:
	virtual uint8_t getEntityTypeId(void) const;

private:
	virtual __int64 queryEntityRenderer(void) const;
	virtual __int64 getSourceUniqueID(void) const;

public:
	virtual void setOnFire(int);

private:
	virtual __int64 getHandleWaterAABB(void) const;
	virtual __int64 handleInsidePortal(vec3_ti const &);
	virtual __int64 getPortalCooldown(void) const;
	virtual __int64 getPortalWaitTime(void) const;

public:
	virtual int* getDimensionId(int* dimOut) const;


	virtual bool canChangeDimensions(void) const;

private:
	virtual __int64 changeDimension(__int64, bool);
	virtual __int64 changeDimension(__int64 const &);
	virtual __int64 getControllingPlayer(void) const;
	virtual __int64 checkFallDamage(float, bool);

public:
	virtual void causeFallDamage(float);
	virtual void handleFallDistanceOnServer(float, bool);

private:
	virtual __int64 playSynchronizedSound(__int64, vec3_t const &, __int64 const &, bool);
	virtual __int64 playSynchronizedSound(__int64, vec3_t const &, int, bool);
	virtual __int64 onSynchedDataUpdate(int);

public:
	virtual bool canAddRider(C_Entity &) const;
	virtual bool canBePulledIntoVehicle(void) const;

private:
	virtual bool returnTrue(void) const;
	virtual __int64 inCaravan(void) const;

public:
	virtual bool isLeashableType(void);

private:
	virtual __int64 tickLeash(void);
	virtual __int64 sendMotionPacketIfNeeded(void);

public:
	virtual bool canSynchronizeNewEntity(void) const;

private:
	virtual __int64 stopRiding(bool, bool, bool);

public:
	virtual void startSwimming(void);
	virtual void stopSwimming(void);

private:
	virtual __int64 buildDebugInfo(std::string &) const;
	virtual __int64 getCommandPermissionLevel(void) const;
	virtual __int64 getMutableAttribute(__int64 const &);

public:
	virtual __int64 getAttribute(int *) const;
	virtual __int64 getDeathTime(void) const;
	virtual __int64 heal(int);
	virtual bool isInvertedHealAndHarm(void) const;
	virtual bool canBeAffected(int) const;
	virtual bool canBeAffected(__int64 const &) const;
	virtual bool canBeAffectedByArrow(__int64 const &) const;

private:
	virtual __int64 onEffectAdded(__int64 &);
	virtual __int64 onEffectUpdated(__int64 const &);
	virtual __int64 onEffectRemoved(__int64 &);
	virtual __int64 getAnimationComponent(void);
	virtual __int64 openContainerComponent(C_Player &);

public:
	virtual __int64 swing(void);

private:
	virtual __int64 useItem(__int64 &, __int64, bool);

public:
	virtual bool hasOutputSignal(unsigned char) const;

private:
	virtual __int64 getOutputSignal(void) const;
	virtual __int64 getDebugText(__int64 &);
	virtual __int64 getMapDecorationRotation(void) const;
	virtual __int64 getRiderYRotation(C_Entity const &) const;
	virtual __int64 getYHeadRot(void) const;

public:
	virtual bool isWorldBuilder(void);
	virtual bool isCreative(void) const;
	virtual bool isAdventure(void) const;

private:
	virtual __int64 add(__int64 &);
	virtual __int64 drop(__int64 const &, bool);
	virtual __int64 getInteraction(C_Player &, __int64 &, vec3_t const &);

public:
	virtual bool canDestroyBlock(__int64 const &) const;
	virtual void setAuxValue(int);
	virtual void setSize(float, float);

private:
	virtual __int64 getLifeSpan(void) const;
	virtual __int64 onOrphan(void);
	virtual __int64 wobble(void);
	virtual __int64 wasHurt(void);

public:
	virtual __int64 startSpinAttack(void);
	virtual __int64 stopSpinAttack(void);

public:
	virtual void setDamageNearbyMobs(bool);

private:
	virtual __int64 renderDebugServerState(__int64 const &);
	virtual __int64 reloadLootTable(void);
	virtual __int64 reloadLootTable(__int64 const *);
	virtual __int64 getDeletionDelayTimeSeconds(void) const;
	virtual __int64 kill(void);
	virtual __int64 die(__int64 const &);
	virtual __int64 shouldTick(void) const;
	virtual __int64 createMovementProxy(void);
	virtual __int64 updateEntitySpecificMolangVariables(__int64 &);

public:
	virtual bool shouldTryMakeStepSound(void) const;
	
private:
	virtual __int64 getNextStep(float) const;
	virtual __int64 idk();
	virtual __int64 outOfWorld(void);
	virtual __int64 _hurt(__int64 const &, int, bool, bool);
	virtual __int64 markHurt(void);
	virtual __int64 readAdditionalSaveData(__int64 const &, __int64 &);
	virtual __int64 addAdditionalSaveData(__int64 &);
	virtual __int64 _playStepSound(vec3_ti const &, __int64 const &);
	virtual __int64 _playFlySound(vec3_ti const &, __int64 const &);
	virtual __int64 _makeFlySound(void) const;
	virtual __int64 checkInsideBlocks(float);
	virtual __int64 pushOutOfBlocks(vec3_t const &);
	virtual __int64 updateWaterState(void);

public:
	virtual __int64 doWaterSplashEffect(void);

private:
	virtual __int64 spawnTrailBubbles(void);
	virtual __int64 updateInsideBlock(void);
	virtual __int64 getLootTable(void);
	virtual __int64 getDefaultLootTable(void);
	virtual __int64 _removeRider(__int64 const &, bool, bool);
	virtual __int64 onSizeUpdated(void);
	virtual __int64 _doAutoAttackOnTouch(C_Entity &);
	virtual __int64 knockback(C_Entity *, int, float, float, float, float, float);
	virtual __int64 resolveDeathLoot(int, __int64 const &);
	virtual __int64 spawnAnim(void);

public:
	virtual void setSleeping(bool);
	virtual void setSprinting(bool);

private:
	virtual __int64 playAmbientSound(void);
	virtual __int64 getAmbientSound(void);
	virtual __int64 getAmbientSoundPostponeTicks(void);
	virtual __int64 getAmbientSoundPostponeTicksRange(void);
	virtual __int64 getItemInHandIcon(__int64 const &, int);

public:
	virtual __int64 getSpeed(void) const;
	virtual void setSpeed(float);

private:
	virtual __int64 getJumpPower(void) const;
	virtual __int64 hurtEffects(__int64 const &, int, bool, bool);
	virtual __int64 getMeleeWeaponDamageBonus(__int64 *);
	virtual __int64 getMeleeKnockbackBonus(void);
	virtual __int64 travel(__int64, float, float, float);
	virtual __int64 travel(float, float, float);
	virtual __int64 applyFinalFriction(float, bool);
	virtual __int64 updateWalkAnim(void);
	virtual __int64 aiStep(__int64);
	virtual __int64 aiStep(void);
	virtual __int64 pushActors(void);
	virtual __int64 lookAt(C_Entity *, float, float);

public:
	virtual bool isLookingAtAnEntity(void);

private:
	virtual __int64 checkSpawnRules(bool);
	virtual __int64 checkSpawnObstruction(void) const;
	virtual __int64 getAttackAnim(float);
	virtual __int64 getItemUseDuration(void);
	virtual __int64 getItemUseStartupProgress(void);
	virtual __int64 getItemUseIntervalProgress(void);
	virtual __int64 getItemuseIntervalAxis(void);
	virtual __int64 getTimeAlongSwing(void);
	virtual __int64 ate(void);
	virtual __int64 getMaxHeadXRot(void);
	virtual __int64 getLastHurtByMob(void);

public:
	virtual void setLastHurtByMob(__int64 *);

private:
	virtual __int64 getLastHurtByPlayer(void);

public:
	virtual void setLastHurtByPlayer(C_Player *);

private:
	virtual __int64 getLastHurtMob(void);

public:
	virtual void setLastHurtMob(C_Entity *);
	virtual bool isAlliedTo(C_Entity *);

private:
	virtual __int64 doHurtTarget(C_Entity *);

public:
	virtual bool canBeControlledByRider(void);

private:
	virtual __int64 leaveCaravan(void);
	virtual __int64 joinCaravan(__int64 *);

public:
	virtual bool hasCaravanTail(void) const;

private:
	virtual __int64 getCaravanHead(void) const;
	virtual __int64 getArmorValue(void);
	virtual __int64 getArmorCoverPercentage(void) const;
	virtual __int64 hurtArmor(int);
	virtual __int64 hurtArmor(int, __int64);
	virtual __int64 hurtArmorSlot(int, __int64);
	virtual __int64 setDamagedArmor();
	virtual __int64 idk3();
	virtual __int64 sendArmor(void);
	virtual __int64 containerChanged(int);
	virtual __int64 updateEquipment(void);
	virtual __int64 clearEquipment(void);
	virtual __int64 getAllArmor(void) const;
	virtual __int64 getAllArmorID(void) const;
	virtual __int64 getAllHand(void) const;
	virtual __int64 getAllEquipment(void) const;
	virtual __int64 getArmorTypeHash(void);
	virtual __int64 dropEquipmentOnDeath(void);
	virtual __int64 dropEquipmentOnDeath(__int64 const &, int);
	virtual __int64 clearVanishEnchantedItems(void);
	virtual __int64 sendInventory(bool);
	virtual __int64 getDamageAfterMagicAbsorb(__int64 const &, int);
	virtual __int64 createAIGoals(void);
	virtual __int64 onBorn(C_Entity &, C_Entity &);
	virtual void setItemSlot(__int64, __int64 const &);

public:
	virtual void setTransitioningSitting(bool);

private:
	virtual __int64 attackAnimation(C_Entity *, float);
	virtual __int64 getAttackTime(void);
	virtual __int64 _getWalkTargetValue(vec3_ti const &);

public:
	virtual bool canExistWhenDisallowMob(void) const;

private:
	virtual void unknown(void) const;
	virtual __int64 ascendLadder(void);
	virtual __int64 ascendScaffolding(void);
	virtual __int64 descendScaffolding(void);
	virtual __int64 dropContainer(void);
	virtual __int64 initBodyControl(void);

public:
	virtual __int64 jumpFromGround0(__int64);
	virtual __int64 jumpFromGround(void);

private:
	virtual __int64 updateAi(void);
	virtual __int64 newServerAiStep(void);
	virtual __int64 _serverAiMobStep(void);
	virtual __int64 getDamageAfterEnchantReduction(__int64 const &, int);
	virtual __int64 getDamageAfterArmorAbsorb(__int64 const &, int);
	virtual __int64 dropBags(void);
	virtual __int64 tickDeath(void);
	virtual __int64 updateGliding(void);
	virtual __int64 _allowAscendingScaffolding(void) const;
	virtual __int64 prepareRegion(__int64 &);
	virtual __int64 destroyRegion(void);
	virtual __int64 suspendRegion(void);
	virtual void resendAllChunks(void);
	virtual __int64 _fireWillChangeDimension(void);
	virtual __int64 _fireDimensionChanged(void);
	virtual __int64 changeDimensionWithCredits(__int64);
	virtual __int64 tickWorld(__int64 const &);

public:
	C_InventoryTransactionManager *getTransactionManager();

	AABB *getAABB() {
		return &this->aabb;
	}

	__int64 *getUniqueId() {
		uintptr_t _this = reinterpret_cast<uintptr_t>(this);
		__int64 *result;  // rax
		__int64 v2;       // rcx

		result = (__int64 *)(_this + 0xE8);
		if (*result == -1i64) {
			v2 = *(__int64 *)(_this + 0x330);
			*result = ++*(__int64 *)(v2 + 0x1A0);
		}
		return result;
	}

	int getTicksUsingItem(){
		return *reinterpret_cast<int*>(reinterpret_cast<__int64>(this) + 0xF60);
	}

	bool isSneaking() {
		return false; // TODO
	}

	bool isSprinting() {
		return false; // TODO
	}

};

#pragma pack(pop)

class C_ServerPlayer;

class C_Player : public C_Entity {
public:
	C_PlayerInventoryProxy *getSupplies();;

	C_ItemStack *getSelectedItem() {
		auto supplies = this->getSupplies();
		return supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
	}

	int getSelectedItemId() {
		auto item = getSelectedItem();
		if (item == nullptr || item->item == nullptr)
			return 0;
		if (item->getItem() == nullptr)
			return 0;
		return item->getItem()->itemId;
	}

private:
	virtual __int64 frameUpdate(__int64 &);
	virtual __int64 getTickingOffsets(void) const;
	virtual __int64 moveView(void);

public:
	virtual void setName(std::string const &);

private:
	virtual __int64 checkMovementStats(vec3_t const &);
	virtual __int64 unknown20(void) const;
	virtual __int64 unknown21(void) const;
	virtual __int64 respawn(void);
	virtual __int64 resetRot(void);
	virtual __int64 resetPos(bool);

public:
	virtual bool isInTrialMode(void);
	virtual bool hasResource(int);

private:
	virtual __int64 completeUsingItem(void);

public:
	virtual void setPermissions(int);

	virtual __int64 startDestroying(void);
	virtual __int64 stopDestroying(void);

public:
	virtual bool returnFalse0() const;
	virtual bool returnFalse1() const;
	
	virtual __int64 openTrading(__int64 const &, bool);

public:
	virtual bool canOpenContainerScreen(void);

private:
	virtual bool returnFalse_0() const;
	virtual bool returnFalse_1() const;

public:
	virtual __int64 openInventory(void);

private:
	virtual bool returnFalse_2() const;
	virtual bool returnFalse_3() const;
	virtual __int64 displayTextObjectMessage(__int64 const &, std::string const &, std::string const &);
	virtual __int64 displayTextObjectMessageWhisperMessage(__int64 const &, std::string const &, std::string const &);
	virtual __int64 displayWhisperMessage(std::string const &, std::string const &, std::string const &, std::string const &);
	virtual __int64 startSleepInBed(vec3_ti const &);
	virtual __int64 stopSleepInBed(bool, bool);

public:
	virtual bool canStartSleepInBed(void);

private:
	virtual __int64 getSleepTimer(void) const;
	virtual __int64 getPreviousTickSleepTimer(void) const;
	virtual bool returnFalse_4() const;
	virtual bool returnFalse_5() const;

public:
	virtual bool returnFalse_6() const;
	virtual bool isHostingC_Player(void) const;
	virtual bool isLoading(void) const;
	virtual bool isC_PlayerInitialized(void) const;

private:
	virtual bool returnFalse_7() const;
	virtual __int64 registerTrackedBoss(__int64);
	virtual __int64 unRegisterTrackedBoss(__int64);

public:
	virtual void setC_PlayerGameType(int);
	virtual __int64 _crit(C_Entity *);

private:
	virtual __int64 getEventing(void) const;

public:
	virtual __int64 getUserId(void) const;

private:
	virtual __int64 sendEventPacket(__int64 &) const;
	virtual __int64 addExperience(int);
	virtual __int64 addLevels(int);

public:
	virtual void setContainerData(__int64 &, int, int);

private:
	virtual __int64 slotChanged(__int64 &, int, C_ItemStack const &, C_ItemStack const &, bool);
	virtual __int64 inventoryChanged(__int64 &, int, C_ItemStack const &, C_ItemStack const &);
	virtual __int64 refreshContainer(__int64 &);
	virtual __int64 deleteContainerManager(void);

public:
	virtual void setFieldOfViewModifier(float);
	virtual bool isPositionRelevant(__int64, int, vec3_ti const &);
	virtual bool isEntityRelevant(C_Entity const &);
	virtual bool isTeacher(void) const;

private:
	virtual __int64 onSuspension(void);
	virtual __int64 onLinkedSlotsChanged(void);
	virtual __int64 startCooldown(C_Item const *);
	virtual __int64 getItemCooldownLeft(int) const;

public:
	virtual bool isItemInCooldown(int) const;
	virtual __int64 sendInventoryTransaction(__int64 const &) const;
	virtual __int64 sendComplexInventoryTransaction(__int64, __int64) const;

private:
	virtual __int64 sendNetworkPacket(__int64 &) const;
	virtual __int64 getC_PlayerEventCoordinator(void);
	virtual __int64 getMoveInputHandler(void);
	virtual __int64 getInputMode(void);
	virtual __int64 getPlayMode(void);
	virtual void **reportMovementTelemetry(__int64 *);
	virtual __int64 onMoveC_PlayerPacketNormal(vec3_t const &, vec2_t const &, float);
};

class C_ServerPlayer : public C_Player {
};

class C_LocalPlayer : public C_Player {
public:

	void unlockAchievments();

	void swingArm() {
		//using SwingArm = void(__thiscall*)(void*);
		//static SwingArm swingFunc = reinterpret_cast<SwingArm>(FindSignature("40 57 48 83 EC ?? 48 C7 44 24 ?? FE FF FF FF 48 89 5C 24 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B D9 80 B9"));
		//swingFunc(this);
		this->swing();
	}
	/*void localPlayerTurn(vec2_t* viewAngles) {
		using Turn = void(__thiscall*)(void*, vec2_t*);
		static Turn TurnFunc = reinterpret_cast<Turn>(FindSignature("4C 8B DC 55 57 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 45 ?? ?? ?? ?? ?? ?? ?? 48 8B 05 D6 D4 A5 01 48 33 C4 48 89 45 ?? 49 89 5B ?? 48 8B F9 ?? ?? ?? ?? ?? ?? ?? 48 8B DA 4D 89 73 E8 41 0F 29 73 D8 41 0F 29 7B C8 48 8B 01 FF 90 ?? ?? ?? ?? F2 0F 10 03"));
		TurnFunc(this, viewAngles);
	}*/
	void applyTurnDelta(vec2_t *viewAngleDelta);
	void setGameModeType(int gma);
};
