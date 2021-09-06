#pragma once
#include "CItem.h"

class C_ItemStack;
class C_Inventory;
class C_Player;

class C_Inventory {
private:
	virtual ~C_Inventory();

public:
	bool isFull();
	void dropSlot(int slot);

	// DROPS WHOLE INVENTORY doesnt work tho
	void dropAll(int slot);

	virtual __int64 init();
	virtual bool serverInitItemStackIds(signed int a2, int a3, __int64 a4);
	virtual __int64 addContentChangeListener(__int64 a2);
	virtual __int64 removeContentChangeListener(__int64 a2);
	virtual C_ItemStack* getItemStack(int slot);
	virtual bool hasRoomForItem(C_ItemStack*);
	virtual __int64 addItem(C_ItemStack*);
	virtual __int64 addItemToFirstEmptySlot(C_ItemStack*);
	virtual __int64 setItem(int slot, const C_ItemStack&);
	virtual __int64 setItemWithForceBalance(int, const C_ItemStack&, bool);
	virtual __int64 removeItem(int, int);
	virtual __int64 removeAllItems();
	virtual __int64 dropContents(C_BlockSource&, const vec3_t&, bool);
	virtual __int64 getContainerSize();
	virtual __int64 getMaxStackSize();
	virtual __int64 startOpen(C_Player&);
	virtual __int64 stopOpen(C_Player&);
	virtual __int64 getSlotCopies();
	virtual __int64 getSlots();
	virtual __int64 getItemCount(const C_ItemStack&);
	virtual __int64 findFirstSlotForItem(const C_ItemStack&);
	virtual __int64 canPushInItem(); // BlockSource &,int,int,ItemInstance const&
	virtual __int64 canPullOutItem();  // BlockSource &,int,int,ItemInstance const&
	virtual __int64 setContainerChanged(int); 
	virtual __int64 setContainerMoved();
	virtual __int64 setCustomName(const TextHolder&); 
	virtual __int64 hasCustomName();
	virtual __int64 readAdditionalSaveData(); //  CompoundTag const&
	virtual __int64 addAdditionalSaveData();   //  CompoundTag &
	virtual __int64 createTransactionContext();  // std::function<void(C_Inventory&, int, const C_ItemStack&, const C_ItemStack&)>, std::function<void(void)>
	virtual __int64 initializeContainerContents(C_BlockSource&);
	virtual char isEmpty();
	virtual __int64 add(C_ItemStack&);
	virtual __int64 canAdd(const C_ItemStack&);
	//virtual void unknown();
	virtual __int64 clearSlot(int);
	virtual __int64 clearInventory(int);
	virtual __int64 load(); // ListTag const&,SemVersion const&,Level &
	//virtual void unknown2();
	virtual __int64 getEmptySlotsCount();
	virtual int getFirstEmptySlot();
	virtual __int64 setContainerSize();

	void moveItem(int from, int to);
	void swapSlots(int from, int to);
};

class C_PlayerInventoryProxy {
private:
	char pad_0x0000[0x10];  //0x0000
public:
	int selectedHotbarSlot;  //0x0010
private:
	char pad_0x0014[0x9C];  //0x0014
public:
	C_Inventory* inventory;  //0x00B0
};

//Im not sure exactly where these unknown's go but the funcs we use work.
class C_ContainerScreenController {
private:
	virtual __int64 destructor();
	virtual __int64 tick(void);
	virtual __int64 handleEvent(__int64&);
	virtual __int64 getRoute(void);
	virtual __int64 setScreenState(__int64, std::string const&);
	virtual __int64 onOpen(void);
	virtual __int64 onTerminate(void);
	virtual __int64 onInit(void);
	virtual __int64 onDelete(void);
	virtual __int64 unknown1();

public:
	virtual bool canExit(void);
	virtual __int64 tryExit(void);

private:
	virtual __int64 areControllerTabsEnabled(void);
	virtual __int64 onCreation(void);
	virtual __int64 logCreationTime(std::string const&, double, double, unsigned char);
	virtual __int64 unknown2();
	virtual __int64 unknown3();

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
	virtual void setAssociatedBlockPos(vec3_ti const&);
	virtual void setAssociatedEntityUniqueID(__int64);
	virtual void setSuspendInput(bool);

private:
	virtual __int64 getCallbackInterval(void);
	virtual __int64 onRender(void);
	virtual __int64 addStaticScreenVars(__int64&);
	virtual __int64 getAdditionalScreenInfo(void);
	virtual __int64 getTelemetryOverride(void);
	virtual __int64 addEventProperties(__int64) const;
	virtual __int64 getSceneType(void);
	virtual __int64 getScreenVersion(void);
	virtual __int64 screenHandlesGamepadMenuButton(void);
	virtual __int64 getProxy(void);
	virtual __int64 onEntered(void);
	virtual __int64 getNameId(std::string const&);
	virtual __int64 _doesScreenHaveExitBehavior(void);
	virtual __int64 _isStillValid(void);
	virtual __int64 _getGamepadHelperVisible(void);
	virtual __int64 _getMixedHelperVisible(void);
	virtual __int64 _getKeyboardHelperVisible(void);
	virtual __int64 _getButtonADescription(void);
	virtual __int64 _getButtonBDescription(void);
	virtual __int64 _getButtonXDescription(void);
	virtual __int64 _getButtonYDescription(void);
	virtual __int64 _getButtonKeyboardDescription(void);

public:
	virtual __int64 _handlePlaceAll(std::string const&, int);
	virtual __int64 _handlePlaceOne(std::string const&, int);
	virtual __int64 _handleSelectSlot(std::string const&, int);
	virtual __int64 _getSelectedSlotInfo(void);
	virtual C_ItemStack* _getItemStack(TextHolder, int) const;

private:
	virtual __int64 _getVisualItemStack(std::string const&, int);
	virtual __int64 _getTakeableItemStackBase(std::string const&, int);
	virtual __int64 _onContainerSlotHovered(std::string const&, int);
	virtual __int64 _onContainerSlotSelected(std::string const&, int);
	virtual __int64 _onContainerSlotPressed(std::string const&, int);
	virtual __int64 _shouldSwap(std::string const&, int, std::string const&, int);
	virtual __int64 _getCollectionName(__int64*);
	virtual __int64 _canSplit(std::string const&, int);
	virtual __int64 _sendFlyingItem(__int64 const&, std::string const&, int, std::string const&, int);

public:
	void handleAutoPlace(uintptr_t a1, std::string name, int slot);
};

class C_CraftingScreenController : public C_ContainerScreenController {
};

class C_ChestScreenController : public C_ContainerScreenController {
};