#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "TextHolder.h"
#include "CEntityList.h"
#include "CBlockLegacy.h"
#include "CInventory.h"
#include "CInventoryTransaction.h"

class C_GameMode;

class PointingStruct
{
private:
	char pad_0x0000[0x630]; //0x0000
public:
	int rayHitType; //0x0570 
	int blockSide; //0x0574 
	vec3_ti block; //0x0578 
	vec3_t rayHitVec; //0x0584 
	C_Entity* entityPtr; //0x0590 

}; //Size=0x0598


class C_Player;

class C_Entity
{
public:
	//uintptr_t** vTable; //0x0000
	uintptr_t* ptrToPtrToEntList; //0x0008 
private:
	char pad_0x0010[0xD0]; //0x0010
public:
	union {
		struct {
			float pitch; //0x00E0
			float yaw; //0x00E4
		};
		vec2_t viewAngles;
	};

	float pitch2; //0x00E8 
	float yaw2; //0x00EC 
private:
	char pad_0x00F0[0x10]; //0x00F0
public:
	vec3_t eyePos0; //0x0100 
private:
	char pad_0x010C[0x5C]; //0x010C
public:
	bool onGround; //0x0168 
private:
	char pad_0x0169[0xF]; //0x0169
public:
	float fallDistance; //0x0178 
private:
	char pad_0x017C[0x84]; //0x017C
public:
	float stepHeight; //0x0200 
private:
	char pad_0x0204[0x14]; //0x204
public:
	float web; //0x218
	// 21C
	char pad_0x021C[0x2];
	bool didEnterWaterBool; // 0x21E
private:
	char pad_0x021D[0x48]; //0x21D
public:
	int ticksAlive; //0x0268 
private:
	char pad_0x026C[0xA8C]; //0x026C
public:
	C_BlockSource* region; //0x0CF8 
private:
	char pad_0x0D00[0x8]; //0x0D00
public:
	PointingStruct* pointingAt; //0x0D08 
private:
	char pad_0x0D10[0xB0]; //0x0D10
public:
	uintptr_t* attribute;//0xDC0
private:
	char pad_0x0DC8[0x20];//0xDC8
public:
	AABB aabb; //0x0DE8 
private:
	char pad_0x0E00[0x4]; //0x0E00
public:
	float width; //0x0E04 
	float height; //0x0E08 
	vec3_t velocity; //0x0E0C 
	vec3_t eyePos1; //0x0E18 
	vec3_t eyePos2; //0x0E24 
private:
	char pad_0x0E30[0x58]; //0x0E30
public:
	uintptr_t entityRuntimeId; //0x0E88 
private:
	char pad_0x0E90[0x30]; //0x0E90
public:
	TextHolder N00000653; //0x0EC0 
private:
	char pad_0x0EE0[0x2E8]; //0x0EE0
public:
	float bodyYaw; //0x11C8 
	float oldBodyYaw; //0x11CC 
	float yawUnused1; //0x11D0 
	float yawUnused2; //0x11D4 
	int damageTime; //0x11D8 
private:
	char pad_0x11DC[0x70]; //0x11DC
public:
	int swingInt; //0x124C 
private:
	char pad_0x1250[0x14]; //0x1250
public:
	int timeSinceDeath; //0x1264 
private:
	char pad_0x1268[0x198]; //0x1268
public:
	TextHolder name2; //0x1400 
private:
	char pad_0x1420[0x500]; //0x1420
public:
	short itemData; //0x1920
	short itemId; //0x1922
private:
	char pad_0x193C[0x1FC]; //0x1924
public:
	C_InventoryTransactionManager transactionManager;//0x1B20
	int gamemode; //0x1BB4 
private:
	char pad_0x1BD0[0x60]; //0x1BD0
public:
	TextHolder uuid; //0x1C30 


private:
	virtual __int64 reloadHardcoded(__int64, __int64 const&);
	virtual __int64 reloadHardcodedClient(__int64, __int64 const&);
	virtual __int64 initializeComponents(__int64, __int64 const&);
	virtual __int64 reloadComponents(__int64, __int64 const&);
public:
	virtual bool hasComponent(__int64 const&)const;
private:
	virtual void destructor();
	// Duplicate destructor
	virtual __int64 reset(void);
	virtual __int64 getOwnerEntityType(void);
	virtual __int64 remove(void);
public:
	virtual void setPos(vec3_t const&);
	virtual vec3_t* getPos(void)const;
	virtual vec3_t* getPosOld(void)const;
	virtual vec3_t* getPosExtrapolated(float)const;
private:
	virtual __int64 getAttachPos(__int64, float)const;
	virtual __int64 getFiringPos(void)const;
public:
	virtual void setRot(vec2_t const&);
private:
	virtual __int64 move(vec3_t const&);
	virtual __int64 getInterpolatedRidingPosition(float)const;
	virtual __int64 getInterpolatedBodyRot(float)const;
	virtual __int64 getInterpolatedHeadRot(float)const;
	virtual __int64 getInterpolatedBodyYaw(float)const;
	virtual __int64 getYawSpeedInDegreesPerSecond(void)const;
	virtual __int64 getInterpolatedWalkAnimSpeed(float)const;
	virtual __int64 getInterpolatedRidingOffset(float)const;
	virtual __int64 checkBlockCollisions(AABB const&);
	virtual __int64 checkBlockCollisions(void);
	virtual __int64 breaksFallingBlocks(void)const;
	virtual __int64 blockedByShield(__int64 const&, C_Entity &);
	virtual __int64 moveRelative(float, float, float, float);
	virtual __int64 teleportTo(vec3_t const&, bool, int, int);
	virtual __int64 tryTeleportTo(vec3_t const&, bool, bool, int, int);
	virtual __int64 lerpTo(vec3_t const&, vec2_t const&, int);
public:
	virtual void lerpMotion(vec3_t const&);
private:
	virtual __int64 getAddPacket(void);
	virtual __int64 normalTick(void);
	virtual __int64 baseTick(void);
	virtual __int64 rideTick(void);
	virtual __int64 positionRider(C_Entity&, float);
	virtual __int64 getRidingHeight(void);
	virtual __int64 startRiding(C_Entity &);
	virtual __int64 addRider(C_Entity &);
	virtual __int64 flagRiderToRemove(C_Entity&);
	virtual __int64 getExitTip(std::string const&, __int64)const;
	virtual __int64 intersects(vec3_t const&, vec3_t const&)const;
public:
	virtual bool isFree(vec3_t const&, float);
	virtual bool isFree(vec3_t const&);
	virtual bool isInWall(void)const;
	virtual bool isInvisible(void)const;
	virtual bool canShowNameTag(void)const;
	virtual bool canExistInPeaceful(void)const;
	virtual void setNameTagVisible(bool);
	virtual TextHolder* getNameTag(void)const;
private:
	virtual __int64 getNameTagAsHash(void)const;
public:
	virtual TextHolder* getFormattedNameTag(void)const;
private:
	virtual __int64 filterFormattedNameTag(__int64 const&);
public:
	virtual void setNameTag(std::string const&);
private:
	virtual __int64 getAlwaysShowNameTag(void)const;
public:
	virtual void setScoreTag(std::string const&);
private:
	virtual __int64 getScoreTag(void)const;
public:
	virtual bool isInWater(void)const;
	virtual bool hasEnteredWater(void)const;
	virtual bool isImmersedInWater(void)const;
	virtual bool isInWaterOrRain(void)const;
	virtual bool isInLava(void)const;
	virtual bool isUnderLiquid(__int64)const;
	virtual bool isOverWater(void)const;
private:
	virtual __int64 makeStuckInBlock(float);
	virtual __int64 getCameraOffset(void)const;
	virtual __int64 getShadowHeightOffs(void);
	virtual __int64 getShadowRadius(void)const;
	virtual __int64 getHeadLookVector(float);
public:
	virtual bool canSeeInvisible(void)const;
	virtual bool canSee(C_Entity const&)const;
	virtual bool canSee(vec3_t const&)const;
	virtual bool isSkyLit(float);
private:
	virtual __int64 getBrightness(float)const;
	virtual __int64 interactPreventDefault(void);
	virtual __int64 playerTouch(C_Player &);
	virtual __int64 push(C_Entity&, bool);
	virtual __int64 push(vec3_t const&);
	virtual __int64 onAboveBubbleColumn(bool);
	virtual __int64 onInsideBubbleColumn(bool);
	virtual __int64 partialPush(vec3_t const&);
public:
	virtual bool isImmobile(void)const;
	virtual bool isSilent(void);
	virtual bool isPickable(void);
	virtual bool isFishable(void)const;
	virtual bool isPushable(void)const;
	virtual bool isPushableByPiston(void)const;
	virtual bool isSleeping(void)const;
	virtual bool isShootable(void);
	virtual bool isSneaking(void)const;
	virtual bool isBlocking(void)const;
	virtual bool isDamageBlocked(__int64 const&)const;
	virtual bool isAlive(void)const;
	virtual bool isOnFire(void)const;
	virtual bool isOnHotBlock(void)const;
	virtual bool isCreativeModeAllowed(void);
	virtual bool isSurfaceMob(void)const;
	virtual bool isTargetable(void)const;
	virtual bool canAttack(C_Entity*, bool)const;
	virtual void setTarget(C_Entity *);
private:
	virtual __int64 findAttackTarget(void);
public:
	virtual bool isValidTarget(C_Entity*)const;
private:
	virtual __int64 attack(C_Entity &);
	virtual __int64 adjustDamageAmount(int &)const;
public:
	virtual void setOwner(__int64);
	virtual void setSitting(bool);
private:
	virtual __int64 onTame(void);
	virtual __int64 onFailedTame(void);
	virtual __int64 getInventorySize(void)const;
	virtual __int64 getEquipSlots(void)const;
	virtual __int64 getChestSlots(void)const;
public:
	virtual void setStanding(bool);
	virtual bool canPowerJump(void)const;
	virtual void setCanPowerJump(bool);
	virtual bool isJumping(void)const;
	virtual bool isEnchanted(void)const;
private:
	virtual __int64 rideJumped(void);
	virtual __int64 rideLanded(vec3_t const&, vec3_t const&);
	virtual __int64 shouldRender(void)const;
public:
	virtual bool isInvulnerableTo(__int64 const&)const;
private:
	virtual __int64 animateHurt(void);
	virtual __int64 doFireHurt(int);
	virtual __int64 onLightningHit(void);
	virtual __int64 onBounceStarted(vec3_ti const&, __int64 const&);
	virtual __int64 feed(int);
	virtual __int64 handleEntityEvent(__int64, int);
	virtual __int64 getPickRadius(void);
	virtual __int64 spawnAtLocation(int, int);
	virtual __int64 spawnAtLocation(int, int, float);
	virtual __int64 spawnAtLocation(__int64 const&, int);
	virtual __int64 spawnAtLocation(__int64 const&, int, float);
	virtual __int64 spawnAtLocation(__int64 const&, float);
	virtual __int64 despawn(void);
	virtual __int64 killed(C_Entity&);
	virtual __int64 awardKillScore(C_Entity &, int);
public:
	virtual void setArmor(int,C_ItemStack const&);
	virtual C_ItemStack* getArmor(int)const;
private:
	virtual __int64 getArmorMaterialTypeInSlot(__int64)const;
	virtual __int64 getArmorMaterialTextureTypeInSlot(__int64)const;
	virtual __int64 getArmorColorInSlot(__int64, int)const;
public:
	virtual void setEquippedSlot(__int64, int, int);
	virtual void setEquippedSlot(__int64, __int64 const&);
private:
	virtual __int64 getCarriedItem(void)const;
public:
	virtual void setCarriedItem(__int64 const&);
	virtual void setOffhandSlot(C_ItemStack*);
	virtual C_ItemStack* getEquippedTotem(void)const;
	virtual __int64 consumeTotem(void);
private:
	virtual __int64 save(__int64 &);
	virtual __int64 saveWithoutId(__int64 &);
	virtual __int64 load(__int64 const&, __int64 &);
	virtual __int64 loadLinks(__int64 const&, __int64);
public:
	virtual int getEntityTypeId(void)const;
private:
	virtual __int64 queryEntityRenderer(void)const;
	virtual __int64 getSourceUniqueID(void)const;
public:
	virtual void setOnFire(int);
private:
	virtual __int64 getHandleWaterAABB(void)const;
	virtual __int64 handleInsidePortal(vec3_ti const&);
	virtual __int64 getPortalCooldown(void)const;
	virtual __int64 getPortalWaitTime(void)const;
	virtual __int64 getDimensionId(void)const;
public:
	virtual bool canChangeDimensions(void)const;
private:
	virtual __int64 changeDimension(__int64, bool);
	virtual __int64 changeDimension(__int64 const&);
	virtual __int64 getControllingC_Player(void)const;
	virtual __int64 checkFallDamage(float, bool);
	virtual __int64 causeFallDamage(float);
	virtual __int64 handleFallDistanceOnServer(float, bool);
	virtual __int64 playSynchronizedSound(__int64, vec3_t const&, __int64 const&, bool);
	virtual __int64 playSynchronizedSound(__int64, vec3_t const&, int, bool);
	virtual __int64 onSynchedDataUpdate(int);
public:
	virtual bool canAddRider(C_Entity &)const;
	virtual bool canBePulledIntoVehicle(void)const;
private:
	virtual __int64 inCaravan(void)const;
public:
	virtual bool isLeashableType(void);
private:
	virtual __int64 tickLeash(void);
	virtual __int64 sendMotionPacketIfNeeded(void);
public:
	virtual bool canSynchronizeNewEntity(void)const;
private:
	virtual __int64 stopRiding(bool, bool, bool);
	virtual __int64 startSwimming(void);
	virtual __int64 stopSwimming(void);
	virtual __int64 buildDebugInfo(std::string &)const;
	virtual __int64 getCommandPermissionLevel(void)const;
	virtual __int64 getMutableAttribute(__int64 const&);
public:
	virtual __int64 getAttribute(int*)const;
	virtual __int64 getDeathTime(void)const;
	virtual __int64 heal(int);
	virtual bool isInvertedHealAndHarm(void)const;
	virtual bool canBeAffected(int)const;
	virtual bool canBeAffected(__int64 const&)const;
	virtual bool canBeAffectedByArrow(__int64 const&)const;
private:
	virtual __int64 onEffectAdded(__int64 &);
	virtual __int64 onEffectUpdated(__int64 const&);
	virtual __int64 onEffectRemoved(__int64 &);
	virtual __int64 openContainerComponent(C_Player &);
public:
	virtual __int64 swing(void);
private:
	virtual __int64 useItem(__int64 &, __int64, bool);
public:
	virtual bool hasOutputSignal(unsigned char)const;
private:
	virtual __int64 getOutputSignal(void)const;
	virtual __int64 getDebugText(__int64 &);
	virtual __int64 getMapDecorationRotation(void)const;
	virtual __int64 getRiderYRotation(C_Entity const&)const;
	virtual __int64 getYHeadRot(void)const;
public:
	virtual bool isWorldBuilder(void);
	virtual bool isCreative(void)const;
	virtual bool isAdventure(void)const;
private:
	virtual __int64 add(__int64 &);
	virtual __int64 drop(__int64 const&, bool);
	virtual __int64 getInteraction(C_Player &, __int64 &, vec3_t const&);
public:
	virtual bool canDestroyBlock(__int64 const&)const;
	virtual void setAuxValue(int);
	virtual void setSize(float, float);
private:
	virtual __int64 getLifeSpan(void)const;
	virtual __int64 onOrphan(void);
	virtual __int64 wobble(void);
	virtual __int64 wasHurt(void);
	virtual __int64 startSpinAttack(void);
	virtual __int64 stopSpinAttack(void);
public:
	virtual void setDamageNearbyMobs(bool);
private:
	virtual __int64 renderDebugServerState(__int64 const&);
	virtual __int64 reloadLootTable(void);
	virtual __int64 reloadLootTable(__int64 const*);
	virtual __int64 getDeletionDelayTimeSeconds(void)const;
	virtual __int64 kill(void);
	virtual __int64 die(__int64 const&);
	virtual __int64 shouldTick(void)const;
	virtual __int64 updateEntitySpecificMolangVariables(__int64 &);
public:
	virtual bool canMakeStepSound(void)const;
private:
	virtual __int64 outOfWorld(void);
	virtual __int64 _hurt(__int64 const&, int, bool, bool);
	virtual __int64 markHurt(void);
	virtual __int64 lavaHurt(void);
	virtual __int64 readAdditionalSaveData(__int64 const&, __int64 &);
	virtual __int64 addAdditionalSaveData(__int64 &);
	virtual __int64 _playStepSound(vec3_ti const&, __int64 const&);
	virtual __int64 _playFlySound(vec3_ti const&, __int64 const&);
	virtual __int64 _makeFlySound(void)const;
	virtual __int64 checkInsideBlocks(float);
	virtual __int64 pushOutOfBlocks(vec3_t const&);
	virtual __int64 updateWaterState(void);
public:
	virtual __int64 doWaterSplashEffect(void);
private:
	virtual __int64 spawnTrailBubbles(void);
	virtual __int64 updateInsideBlock(void);
	virtual __int64 getLootTable(void);
	virtual __int64 getDefaultLootTable(void);
	virtual __int64 _removeRider(__int64 const&, bool, bool);
	virtual __int64 onSizeUpdated(void);
	virtual __int64 _doAutoAttackOnTouch(C_Entity&);
	virtual __int64 knockback(C_Entity *, int, float, float, float, float, float);
	virtual __int64 resolveDeathLoot(int, __int64 const&);
	virtual __int64 spawnAnim(void);
public:
	virtual void setSneaking(bool);
	virtual bool isSprinting(void)const;
	virtual void setSprinting(bool);
private:
	virtual __int64 getVoicePitch(void);
	virtual __int64 playAmbientSound(void);
	virtual __int64 getAmbientSound(void);
	virtual __int64 getAmbientSoundPostponeTicks(void);
	virtual __int64 getItemInHandIcon(__int64 const&, int);
public:
	virtual __int64 getSpeed(void)const;
	virtual void setSpeed(float);
private:
	virtual __int64 getJumpPower(void)const;
	virtual __int64 hurtEffects(__int64 const&, int, bool, bool);
	virtual __int64 getMeleeWeaponDamageBonus(__int64*);
	virtual __int64 getMeleeKnockbackBonus(void);
	virtual __int64 actuallyHurt(int, __int64 const*, bool);
	virtual __int64 travel(float, float, float);
	virtual __int64 applyFinalFriction(float, bool);
	virtual __int64 updateWalkAnim(void);
	virtual __int64 aiStep(void);
	virtual __int64 pushEntities(void);
	virtual __int64 lookAt(C_Entity *, float, float);
public:
	virtual bool isLookingAtAnEntity(void);
private:
	virtual __int64 checkSpawnRules(bool);
	virtual __int64 checkSpawnObstruction(void)const;
	virtual __int64 shouldDespawn(void)const;
	virtual __int64 getAttackAnim(float);
	virtual __int64 performRangedAttack(C_Entity &, float);
	virtual __int64 getItemUseDuration(void);
	virtual __int64 getItemUseStartupProgress(void);
	virtual __int64 getItemUseIntervalProgress(void);
	virtual __int64 getItemuseIntervalAxis(void);
	virtual __int64 getTimeAlongSwing(void);
	virtual __int64 ate(void);
	virtual __int64 getMaxHeadXRot(void);
	virtual __int64 getLastHurtByMob(void);
public:
	virtual void setLastHurtByMob(__int64*);
private:
	virtual __int64 getLastHurtByC_Player(void);
public:
	virtual void setLastHurtByC_Player(C_Player *);
private:
	virtual __int64 getLastHurtMob(void);
public:
	virtual void setLastHurtMob(C_Entity *);
	virtual bool isAlliedTo(__int64*);
private:
	virtual __int64 doHurtTarget(C_Entity *);
public:
	virtual bool canBeControlledByRider(void);
private:
	virtual __int64 leaveCaravan(void);
	virtual __int64 joinCaravan(__int64*);
public:
	virtual bool hasCaravanTail(void)const;
private:
	virtual __int64 getCaravanHead(void)const;
	virtual __int64 getEquipmentCount(void)const;
	virtual __int64 getArmorValue(void);
	virtual __int64 getArmorCoverPercentage(void)const;
	virtual __int64 hurtArmor(int);
	virtual __int64 containerChanged(int);
	virtual __int64 updateEquipment(void);
	virtual __int64 clearEquipment(void);
	virtual __int64 getAllArmor(void)const;
	virtual __int64 getAllArmorID(void)const;
	virtual __int64 getAllHand(void)const;
	virtual __int64 getAllEquipment(void)const;
	virtual __int64 getArmorTypeHash(void);
	virtual __int64 sendInventory(bool);
	virtual __int64 sendArmor(void);
	virtual __int64 getDamageAfterMagicAbsorb(__int64 const&, int);
	virtual __int64 createAIGoals(void);
	virtual __int64 onBorn(C_Entity &, C_Entity &);
	virtual void setItemSlot(__int64, __int64 const&);
	virtual __int64 goDownInWater(void);
public:
	virtual void setTransitioningSitting(bool);
private:
	virtual __int64 attackAnimation(C_Entity *, float);
	virtual __int64 getAttackTime(void);
	virtual __int64 _getWalkTargetValue(vec3_ti const&);
public:
	virtual bool canExistWhenDisallowMob(void)const;
private:
	virtual __int64 ascendLadder(void);
	virtual __int64 ascendScaffolding(void);
	virtual __int64 descendScaffolding(void);
	virtual __int64 initBodyControl(void);
public:
	virtual __int64 jumpFromGround(void);
private:
	virtual __int64 updateAi(void);
	virtual __int64 newServerAiStep(void);
	virtual __int64 _serverAiMobStep(void);
	virtual __int64 getDamageAfterEnchantReduction(__int64 const&, int);
	virtual __int64 getDamageAfterArmorAbsorb(__int64 const&, int);
	virtual __int64 getExperienceReward(void)const;
	virtual __int64 dropEquipment(__int64 const&, int);
	virtual __int64 dropEquipment(void);
	virtual __int64 dropBags(void);
	virtual __int64 dropContainer(void);
	virtual __int64 useNewAi(void)const;
	virtual __int64 tickDeath(void);
	virtual __int64 _endJump(void);
	virtual __int64 updateGliding(void);
	virtual __int64 _allowAscendingScaffolding(void)const;
	virtual __int64 prepareRegion(__int64 &);
	virtual __int64 destroyRegion(void);
	virtual __int64 suspendRegion(void);
	virtual __int64 _fireWillChangeDimension(void);
	virtual __int64 _fireDimensionChanged(void);
	virtual __int64 changeDimensionWithCredits(__int64);
	virtual __int64 tickWorld(__int64 const&);
public:

	AABB* getAABB() {
		uintptr_t _this = reinterpret_cast<uintptr_t>(this);
		static int AABBOffset = 0x0;
		if (AABBOffset == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("F3 ?? ?? ?? ?? ?? ?? ?? 41 0F 2F 00 F3 0F 10 2A");
			if (sigOffset != 0x0) {
				AABBOffset = *reinterpret_cast<int*>((sigOffset + 4)); // Get Offset from code
			}
		}
		AABB* aabb;
		aabb = reinterpret_cast<AABB*>(_this + AABBOffset);
		return aabb;
	}

	bool isInventoryClosed(){
		return Utils::CallVFunc<387,bool>(this);
	};
};

class C_ServerPlayer;

class C_Player : public C_Entity {
public:
	C_PlayerInventoryProxy* getSupplies() {
		return *reinterpret_cast<C_PlayerInventoryProxy**>(reinterpret_cast<__int64>(this) + 0x17A8);
	};

	C_ServerPlayer* getServerPlayer() {
		return *reinterpret_cast<C_ServerPlayer**>(reinterpret_cast<__int64>(this) + 0x1BC0);
	};
private:
	virtual __int64 frameUpdate(__int64&);
	virtual __int64 getTickingOffsets(void)const;
	virtual __int64 moveView(void);
	virtual __int64 moveSpawnView(vec3_t const&);
public:
	virtual void setName(std::string const&);
private:
	virtual __int64 checkMovementStats(vec3_t const&);
	virtual __int64 getCurrentStructureFeature(void)const;
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
private:
	virtual __int64 startCrafting(vec3_ti const&, bool);
	virtual __int64 startStonecutting(vec3_ti const&);
	virtual __int64 startDestroying(void);
	virtual __int64 stopDestroying(void);
	virtual __int64 openContainer(vec3_ti const&);
	virtual __int64 openContainer(__int64 const&);
	virtual __int64 openFurnace(vec3_ti const&);
	virtual __int64 openBlastFurnace(vec3_ti const&);
	virtual __int64 openSmoker(vec3_ti const&);
	virtual __int64 openEnchanter(vec3_ti const&);
	virtual __int64 openAnvil(vec3_ti const&);
	virtual __int64 openGrindstone(vec3_ti const&);
	virtual __int64 openBrewingStand(vec3_ti const&);
	virtual __int64 openHopper(vec3_ti const&);
	virtual __int64 openHopper(__int64 const&);
	virtual __int64 openDispenser(vec3_ti const&, bool);
	virtual __int64 openBeacon(vec3_ti const&);
	virtual __int64 openPortfolio(void);
	virtual __int64 openBook(int, bool, int, __int64*);
	virtual __int64 openCommandBlock(vec3_ti const&);
	virtual __int64 openCommandBlockMinecart(__int64 const&);
	virtual __int64 openHorseInventory(__int64 const&);
	virtual __int64 openTrading(__int64 const&, bool);
public:
	virtual bool canOpenContainerScreen(void);
private:
	virtual __int64 openChalkboard(__int64&, bool);
	virtual __int64 openNpcInteractScreen(C_Entity&);
	virtual __int64 openInventory(void);
	virtual __int64 openStructureEditor(vec3_ti const&);
	virtual __int64 openLabTable(vec3_ti const&);
	virtual __int64 openElementConstructor(vec3_ti const&);
	virtual __int64 openCompoundCreator(vec3_ti const&);
	virtual __int64 openMaterialReducer(vec3_ti const&);
	virtual __int64 openLoom(vec3_ti const&);
	virtual __int64 openStonecutter(vec3_ti const&);
	virtual __int64 openCartographyTable(vec3_ti const&);
	virtual __int64 displayChatMessage(std::string const&, std::string const&);
	virtual __int64 displayClientMessage(std::string const&);
	virtual __int64 displayLocalizableMessage(std::string const&, __int64 const&);
	virtual __int64 displayTextObjectMessage(__int64 const&, std::string const&, std::string const&);
	virtual __int64 displayWhisperMessage(std::string const&, std::string const&, std::string const&, std::string const&);
	virtual __int64 startSleepInBed(vec3_ti const&);
	virtual __int64 stopSleepInBed(bool, bool);
public:
	virtual bool canStartSleepInBed(void);
private:
	virtual __int64 getSleepTimer(void)const;
	virtual __int64 getPreviousTickSleepTimer(void)const;
	virtual __int64 openSign(vec3_ti const&);
public:
	virtual bool isLocalC_Player(void)const;
	virtual bool isHostingC_Player(void)const;
	virtual bool isLoading(void)const;
	virtual bool isC_PlayerInitialized(void)const;
private:
	virtual __int64 stopLoading(void);
	virtual __int64 registerTrackedBoss(__int64);
	virtual __int64 unRegisterTrackedBoss(__int64);
public:
	virtual void setC_PlayerGameType(int);
	virtual __int64 _crit(C_Entity*);
private:
	virtual __int64 getEventing(void)const;
	virtual __int64 getUserId(void)const;
	virtual __int64 sendEventPacket(__int64&)const;
	virtual __int64 addExperience(int);
	virtual __int64 addLevels(int);
public:
	virtual void setContainerData(__int64&, int, int);
private:
	virtual __int64 slotChanged(__int64&, int, C_ItemStack const&, C_ItemStack const&, bool);
	virtual __int64 inventoryChanged(__int64&, int, C_ItemStack const&, C_ItemStack const&);
	virtual __int64 refreshContainer(__int64&);
	virtual __int64 deleteContainerManager(void);
public:
	virtual void setFieldOfViewModifier(float);
	virtual bool isPositionRelevant(__int64, int, vec3_ti const&);
	virtual bool isEntityRelevant(C_Entity const&);
	virtual bool isTeacher(void)const;
private:
	virtual __int64 onSuspension(void);
	virtual __int64 onLinkedSlotsChanged(void);
	virtual __int64 startCooldown(C_Item const*);
	virtual __int64 getItemCooldownLeft(int)const;
public:
	virtual bool isItemInCooldown(int)const;
	virtual __int64 sendInventoryTransaction(__int64 const&)const;
	virtual __int64 sendComplexInventoryTransaction(__int64, __int64)const;
private:
	virtual __int64 sendNetworkPacket(__int64&)const;
	virtual __int64 chorusFruitTeleport(vec3_t&);
	virtual __int64 getC_PlayerEventCoordinator(void);
	virtual __int64 onMoveC_PlayerPacketNormal(vec3_t const&, vec2_t const&, float);
};

class C_ServerPlayer : public C_Player {
public:
	void sendInventory() {
		
		//Utils::CallVFunc<315, void>(this);
	}
};

class C_LocalPlayer : public C_Player {
public:
	void sendInventory() {
		//using sendInventory_t = void(__fastcall*)(C_Player*, bool);
		//static sendInventory_t sendInventory = reinterpret_cast<sendInventory_t>(Utils::FindSignature("48 8B C4 55 48 8D A8 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? FE FF FF FF 48 89 58 ?? 48 89 70 ?? 48 89 78 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 0F B6 DA 48 8B F1"));
		//sendInventory(this, true);
	};

	void unlockAchievments() { // MinecraftEventing::fireEventAwardAchievement
		using fireEventAward = void(__fastcall*)(void*, int);
		static fireEventAward fireEventAwardFunc = reinterpret_cast<fireEventAward>(Utils::FindSignature("48 85 C9 0F 84 ?? ?? ?? ?? 55 56 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? FE FF FF FF 48 89 9C 24 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 8B"));
		for (int i = 0; i < 100; i++)
			fireEventAwardFunc(this, i);
	}

	void swingArm() {
		using SwingArm = void(__thiscall*)(void*);
		static SwingArm swingFunc = reinterpret_cast<SwingArm>(Utils::FindSignature("40 57 48 83 EC ?? 48 C7 44 24 ?? FE FF FF FF 48 89 5C 24 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B D9 80 B9"));
		swingFunc(this);
	}
	void localPlayerTurn(vec2_t* viewAngles) {
		using Turn = void(__thiscall*)(void*, vec2_t*);
		static Turn TurnFunc = reinterpret_cast<Turn>(Utils::FindSignature("4C 8B DC 55 57 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 45 ?? ?? ?? ?? ?? ?? ?? 48 8B 05 D6 D4 A5 01 48 33 C4 48 89 45 ?? 49 89 5B ?? 48 8B F9 ?? ?? ?? ?? ?? ?? ?? 48 8B DA 4D 89 73 E8 41 0F 29 73 D8 41 0F 29 7B C8 48 8B 01 FF 90 ?? ?? ?? ?? F2 0F 10 03"));
		TurnFunc(this, viewAngles);
	}
	void applyTurnDelta(vec2_t* viewAngleDelta)
	{
		using applyTurnDelta = void(__thiscall*)(void*, vec2_t*);
		//	static applyTurnDelta TurnDelta = reinterpret_cast<applyTurnDelta>(Utils::FindSignature("48 89 5C 24 10 57 48 83 EC ?? 0F 29 74 24 ?? 48 8B FA F3 0F 10 35 16 AA 35 01 48 8B D9 0F 29 7C 24 ?? 0F 28 CE F3 0F 10 7A ?? 0F 28 C7 44 0F 29 44 24 ?? F3 ?? ?? ?? ?? ?? ?? ?? F3 44 0F 10 05 88 A9 35 01 44 0F 29 4C 24 ?? F3 41 0F 58 C0"));
		static applyTurnDelta TurnDelta = reinterpret_cast<applyTurnDelta>(Utils::FindSignature("48 89 5C 24 10 57 48 83 EC ?? 0F 29 74 24 ?? 48 8B FA F3 ?? ?? ?? ?? ?? ?? ?? 48 8B D9 0F 29 7C 24 ?? 0F 28 CE F3 0F 10 7A ?? 0F 28 C7 44 0F 29 44 24 ?? F3 ?? ?? ?? ?? ?? ?? ?? F3 ?? ?? ?? ?? ?? ?? ?? ?? 44 0F 29 4C 24 ?? F3 41 0F 58 C0"));
		TurnDelta(this, viewAngleDelta);
	}
	void setGameModeType(int GameMode)
	{
		using setGameMode = void(__thiscall*)(void*,int);
		static setGameMode Game_Mode = reinterpret_cast<setGameMode>(Utils::FindSignature("40 57 48 83 EC ?? 48 C7 44 24 ?? FE FF FF FF 48 89 5C 24 ?? 48 89 74 24 ?? 8B DA 48 8B F9 8B"));
		Game_Mode(this,GameMode);
	}
};