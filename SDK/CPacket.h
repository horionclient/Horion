#pragma once
#include "../Utils/HMath.h"
#include "CEntity.h"
#include "CComplexInventoryTransaction.h"
#include "TextHolder.h"

class C_Packet {
public:
	uintptr_t** vTable;  //0x0000
	TextHolder* getName();
	template <class T>
	bool isInstanceOf() {
		T packet;
		if (packet.vTable == this->vTable)
			return true;
		else
			return false;
	}
};

class LevelSoundEventPacket : public C_Packet {
private:
	char pad_[0x20];  //0x8
public:
	int sound;           //0x28
	vec3_t pos;          //0x2C
	int extraData = -1;  //0x38
private:
	int unknown = 0;  //0x3C
public:
	TextHolder entityType;               //0x40
	bool isBabyMod = false;              //0x60
	bool disableRelativeVolume = false;  //0x61
	LevelSoundEventPacket();
};

class PlayerAuthInputPacket : public C_Packet {
private:
	char pad_0x0000[0x18];  //0x0008
public:
	__int64 entityRuntimeId;  //0x0020
	float pitch;              //0x0028
	float yaw;                //0x002C
	vec3_t pos;               //0x0030
	float yawUnused;          //0x003C
private:
	char pad_0x0040[0x20];  //0x0040
public:
	PlayerAuthInputPacket();
	PlayerAuthInputPacket(__int64 entityRuntimeId, vec3_t pos, float pitch, float yaw, float yawUnused);
};

/*class C_ActorFallPacket : public C_Packet {
private:
	char pad_0x8[0x20];  //0x8
public:
	C_ActorFallPacket();
	__int64 runtimeId;  // 0x28
	float fallDistance;
	bool isInVoid;
};*/

class C_PlayerActionPacket : public C_Packet {
public:
	C_PlayerActionPacket();
	char pad_0x8[0x20];  //0x8
	vec3_ti blockPosition; // 0x28
	int face; // 0x34
	int action; // 0x38
	__int64 entityRuntimeId; // 0x40
};


class C_MobEquipmentPacket : public C_Packet {
public:
	C_MobEquipmentPacket();
	C_MobEquipmentPacket(__int64 entityRuntimeId, C_ItemStack& item, int hotbarSlot, int inventorySlot);

private:
	char pad_0x8[0x20];       //0x0
public:
	__int64 entityRuntimeId;  //0x28
	C_ItemStack item;         //0x30
	int inventorySlot;        // 0xB8 DONT USE FOR PACKET SENDING
	int hotbarSlot;           // 0xBC DONT USE FOR PACKET SENDING
	char windowId;            //0xC0  DONT USE FOR PACKET SENDING
	char windowId1;           //0xC1  DONT USE FOR PACKET SENDING
	char inventorySlot1;      //0xC2
	char hotbarSlot1;         //0xC3
	char windowId2;           //0xC4 ALL OF THIS IS PROBABLY BROKEN, DONT USE
private:
	char unknown1;
};

class C_InventoryTransactionPacket : public C_Packet {
public:
	C_InventoryTransactionPacket();
	C_InventoryTransactionPacket(C_ComplexInventoryTransaction* transac) : C_InventoryTransactionPacket() {
		this->complexTransaction = transac;
	}

private:
	char pad_0x8[0x20];  //0x8
public:
	int unknown = 0;//0x28
	__int64* unknownStart = 0;  //0x30
	__int64* unknownEnd = 0; //0x38
	__int64 filler = 0;         // 0x40
	C_ComplexInventoryTransaction* complexTransaction; // 0x48
	unsigned char numTransactions; // 0x50
};

class C_TextPacket : public C_Packet {
public:
	C_TextPacket();

	unsigned char gap0[0x20];
	unsigned __int8 messageType;  // 0x28

	unsigned char gap[7];

	TextHolder sourceName;           // 0x30
	TextHolder message;              // 0x50
	unsigned char field_24222[24];   // 0x70
	bool translationNeeded = false;  // 0x88

	unsigned char ga2p[7];  // 0x89
	TextHolder xboxUserId;
	TextHolder platformChatId;
};

#pragma pack(push, 8)

__declspec(align(8)) class C_MovePlayerPacket : public C_Packet {
public:
	C_MovePlayerPacket();
	C_MovePlayerPacket(C_LocalPlayer* player, vec3_t pos);

	//uintptr_t** vTable;		// 0x0
private:
	char filler[0x20];  // 0x8
public:
	__int64 entityRuntimeID;  //0x28
	vec3_t Position;          //0x30
	float pitch;              //0x3c
	float yaw;                //0x40
	float headYaw;            //0x44
	uint8_t mode;             //0x48
	bool onGround;
	__int64 ridingEid;
	int int1;
	int int2;
};

#pragma pack(pop)
