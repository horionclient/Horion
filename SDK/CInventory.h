#pragma once
#include "CEntity.h"
#include "CItem.h"

class C_Inventory {
private:
	virtual ~C_Inventory();
public:
	bool isFull() {
		int fullslots = 0;
		for (int i = 0; i < 36; i++) {
			if (this->getItemStack(i)->item != nullptr)
				fullslots++;
		}
		if (fullslots == 36) return true;
		return false;
	}
	int getFirstEmptySlot() {
		for (int i = 0; i < 36; i++) {
			if (this->getItemStack(i)->item == nullptr)
				return i;
		}
		return -1;
	}
	void dropSlot(int slot) {
		// FillingContainer::dropSlot
		using drop_t = void(__fastcall*)(C_Inventory*, int, char);
		static drop_t func = reinterpret_cast<drop_t>(Utils::FindSignature("85 D2 0F 88 ?? ?? ?? ?? 55 56 57 41 54 41 55 41 56 41 57 48"));
		if (func != 0)
			func(this, slot,0);
	}

	// DROPS WHOLE INVENTORY doesnt work tho
	void dropAll(int slot) {
		// FillingContainer::dropAll
		using dropAll_t = void(__fastcall*)(C_Inventory*, int, int, char);
		static dropAll_t func = reinterpret_cast<dropAll_t>(Utils::FindSignature("48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 48 89 7C 24 ?? 41 56 48 83 EC ?? 48 8B 01 41 0F"));
		if (func != 0)
			func(this, slot, 0, 0);
	}

	virtual __int64 init();
	virtual __int64 addContentChangeListener(__int64 a2);
	virtual __int64 removeContentChangeListener(__int64 a2);
	virtual C_ItemStack* getItemStack(int slot);
	virtual bool hasRoomForItem(C_ItemStack*);
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

class C_ContainerScreenController {
public:
	void handleAutoPlace(uintptr_t a1, std::string name, int slot) {
		using ContainerScreenController__autoPlace = __int64(__fastcall*)(C_ContainerScreenController*, uintptr_t, TextHolder, int);
		static ContainerScreenController__autoPlace autoPlaceFunc = reinterpret_cast<ContainerScreenController__autoPlace>(Utils::FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 ?? ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? 45 8B E1"));

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

class C_CraftingScreenController : public C_ContainerScreenController {

};

class C_ChestScreenController : public C_ContainerScreenController {

};