class C_Item {
private:
	uintptr_t** Vtable;//0x0
	char pad_0x0008[0x68];//0x8
public:
	uint16_t itemId;//0x70
private:
	char pad_0x00072[0x6];//0x72
public:
	TextHolder tileName;//0x78
	TextHolder name;//0x98
};

class C_ItemStack {
public:
	C_Item** item;//0x0
};

class C_Inventory {
private:
	virtual ~C_Inventory();
public:
	virtual __int64 addContentChangeListener(__int64 a2);
	virtual __int64 removeContentChangeListener(__int64 a2);
	virtual C_ItemStack* getItemStack(int slot);
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