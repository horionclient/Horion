#pragma once
#include "CItem.h"

class C_ItemStack;

class C_Inventory {
private:
	virtual ~C_Inventory();

public:
	bool isFull();
	int getFirstEmptySlot();
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

	void moveItem(int from, int to);
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

class C_ContainerScreenController {
public:
	void handleAutoPlace(uintptr_t a1, std::string name, int slot);

private:
	virtual __int64 destructor();
	virtual __int64 tick(void);
	virtual __int64 handleEvent(__int64&);
	virtual __int64 sub_140321330(void) const;
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
	virtual __int64 getCallbackInterval(void) const;

public:
	virtual void setViewCommand(__int64);

private:
	virtual __int64 onRender(void);
	virtual __int64 addStaticScreenVars(__int64&);
	virtual __int64 getAdditionalScreenInfo(void) const;
	virtual __int64 getTelemetryOverride(void) const;
	virtual __int64 addEventProperties(__int64) const;
	virtual __int64 getSceneType(void) const;
	virtual __int64 getScreenVersion(void) const;
	virtual __int64 screenHandlesGamepadMenuButton(void) const;
	virtual __int64 getProxy(void);
	virtual __int64 onEntered(void);
	virtual __int64 getNameId(std::string const&) const;
	virtual __int64 _isStillValid(void) const;
	virtual __int64 _getGamepadHelperVisible(void) const;
	virtual __int64 _getMixedHelperVisible(void) const;
	virtual __int64 _getKeyboardHelperVisible(void) const;
	virtual __int64 _getButtonADescription(void);
	virtual __int64 _getButtonBDescription(void);
	virtual __int64 _getButtonXDescription(void);
	virtual __int64 _getButtonYDescription(void);
	virtual __int64 _getButtonKeyboardDescription(void);

public:
	virtual __int64 _handlePlaceAll(std::string const&, int);
	virtual __int64 _handlePlaceOne(std::string const&, int);
	virtual __int64 _handleSelectSlot(std::string const&, int);
	virtual __int64 _getSelectedItem(void) const;
	virtual C_ItemStack* _getItemStack(TextHolder, int) const;

private:
	virtual __int64 _getVisualItemStack(std::string const&, int) const;
	virtual __int64 _onContainerSlotHovered(std::string const&, int);
	virtual __int64 _onContainerSlotSelected(std::string const&, int);
	virtual __int64 _onContainerSlotPressed(std::string const&, int);
	virtual __int64 _shouldSwap(std::string const&, int, std::string const&, int) const;
	virtual __int64 _getCollectionName(__int64*) const;
	virtual __int64 _canSplit(std::string const&, int) const;
	virtual __int64 _sendFlyingItem(__int64 const&, std::string const&, int, std::string const&, int);
};

class C_CraftingScreenController : public C_ContainerScreenController {
};

class C_ChestScreenController : public C_ContainerScreenController {
};