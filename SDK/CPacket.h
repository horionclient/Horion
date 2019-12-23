#pragma once
#include "../Utils/HMath.h"
#include "../Utils/Logger.h"
#include "TextHolder.h"
#include "CComplexInventoryTransaction.h"

class C_Packet {
public:
	uintptr_t** vTable; //0x0000
	TextHolder* getName() {
		return Utils::CallVFunc<2, TextHolder*>(this, new TextHolder());
	}
	template <class T>
	bool isInstanceOf() {
		T packet;
		if (packet.vTable == this->vTable)
			return true;
		else
			return false;
	}
};

class PlayerAuthInputPacket : public C_Packet {
private:
	char pad_0x0000[0x18]; //0x0008
public:
	__int64 entityRuntimeId; //0x0020
	float pitch; //0x0028
	float yaw; //0x002C
	vec3_t pos; //0x0030
	float yawUnused;//0x003C
private:
	char pad_0x0040[0x20]; //0x0040
public:
	PlayerAuthInputPacket() {
		static uintptr_t** PlayerAuthInputPacketVtable = 0x0;
		if (PlayerAuthInputPacketVtable == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("48 8D 15 ?? ?? ?? ?? 48 89 11 48 89 79 ?? 48 89 79 ?? 89 79 ?? 48 89 79 ?? 48 89 79 ??");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			PlayerAuthInputPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (PlayerAuthInputPacketVtable == 0x0 || sigOffset == 0x0)
				logF("PlayerAuthInputPacketVtable signature not working!!!");
		}
		memset(this, 0, sizeof(PlayerAuthInputPacket)); // Avoid overwriting vtable
		vTable = PlayerAuthInputPacketVtable;
	}
	PlayerAuthInputPacket(__int64 entityRuntimeId,vec3_t pos,float pitch,float yaw,float yawUnused) {
		static uintptr_t** PlayerAuthInputPacketVtable = 0x0;
		if (PlayerAuthInputPacketVtable == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("48 8D 15 ?? ?? ?? ?? 48 89 11 48 89 79 ?? 48 89 79 ?? 89 79 ?? 48 89 79 ?? 48 89 79 ??");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			PlayerAuthInputPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (PlayerAuthInputPacketVtable == 0x0 || sigOffset == 0x0)
				logF("PlayerAuthInputPacketVtable signature not working!!!");
		}
		memset(this, 0, sizeof(PlayerAuthInputPacket)); // Avoid overwriting vtable
		vTable = PlayerAuthInputPacketVtable;
		this->pos = pos;
		this->pitch = pitch;
		this->yaw = yaw;
		this->yawUnused = yawUnused;
		this->entityRuntimeId = entityRuntimeId;
	}
};

class C_ActorFallPacket : public C_Packet {
private:
	char pad_0x8[0x20];//0x8
public:
	C_ActorFallPacket() {
		static uintptr_t** ActorFallPacketVtable = 0x0;
		if (ActorFallPacketVtable == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("48 8D 0D ?? ?? ?? ?? 48 89 4D ?? 48 89 45 EF F3 44 0F 11 45 F7 88 5D FB");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			ActorFallPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (ActorFallPacketVtable == 0x0 || sigOffset == 0x0)
				logF("C_ActorFallPacketVtable signature not working!!!");
		}
		memset(this, 0, sizeof(C_ActorFallPacket)); // Avoid overwriting vtable
		vTable = ActorFallPacketVtable;
	}
	__int64 runtimeId; // 0x28
	float fallDistance;
	bool isInVoid;
};

class C_MobEquipmentPacket : public C_Packet {
public:
	C_MobEquipmentPacket() {
		static uintptr_t** MobEquipmentPacketVtable = 0x0;
		if (MobEquipmentPacketVtable == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 01 48 89 51 ?? 48 83 C1 28 49 8B D0");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			MobEquipmentPacketVtable = reinterpret_cast<uintptr_t * *>(sigOffset + offset + /*length of instruction*/ 7);
			if (MobEquipmentPacketVtable == 0x0 || sigOffset == 0x0)
				logF("C_MobEquipmentPacketVtable signature not working!!!");
		}
		memset(this, 0, sizeof(C_MobEquipmentPacket)); // Avoid overwriting vtable
		vTable = MobEquipmentPacketVtable;
	}
	C_MobEquipmentPacket(__int64 entityRuntimeId,C_ItemStack& item, int hotbarSlot, int inventorySlot) {
		memset(this, 0x0, sizeof(C_MobEquipmentPacket));
		using MobEquimentPacketConstructor_t = void(__fastcall*)(C_MobEquipmentPacket*,__int64, C_ItemStack&, int, int, char);
		static MobEquimentPacketConstructor_t MobEquimentPacketConstructor = reinterpret_cast<MobEquimentPacketConstructor_t>(Utils::FindSignature("48 89 4C 24 08 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 48 41 8B D9 48 8B F9 C7 41 ?? ?? ?? ?? ?? C7 41 ?? ?? ?? ?? ?? C6 41 ?? 00 48 ?? ?? ?? ?? ?? ?? ?? 48 8D 05 ?? ?? ?? ??"));
		if (MobEquimentPacketConstructor != 0)
			MobEquimentPacketConstructor(this, entityRuntimeId,item, hotbarSlot, inventorySlot, 0);
	}
private:
	char pad_0x8[0x20];//0x0
//public:
	__int64 entityRuntimeId;//0x28
	C_ItemStack item;//0x30
	int inventorySlot; // 0xB8
	int hotbarSlot; // 0xBC
	char windowId;//0xC0
	char windowId1;//0xC0
	char inventorySlot1; //0xC4
	char hotbarSlot1; //0xC3
	char windowId2;//0xC4 ALL OF THIS IS PROBABLY BROKEN, DONT USE
private:
	char unknown1;
};

class C_InventoryTransactionPacket : public C_Packet {
public:
	C_InventoryTransactionPacket() {
		static uintptr_t** InventoryTransactionPacketVtable = 0x0;
		if (InventoryTransactionPacketVtable == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("48 8D 15 ?? ?? ?? ?? 49 89 53 C0 49 89 43");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			InventoryTransactionPacketVtable = reinterpret_cast<uintptr_t * *>(sigOffset + offset + /*length of instruction*/ 7);
			if (InventoryTransactionPacketVtable == 0x0 || sigOffset == 0x0)
				logF("C_InventoryTransactionPacketVtable signature not working!!!");
		}
		memset(this, 0, sizeof(C_InventoryTransactionPacket)); // Avoid overwriting vtable
		vTable = InventoryTransactionPacketVtable;
	}
	C_InventoryTransactionPacket(C_ComplexInventoryTransaction* transac) : C_InventoryTransactionPacket() {
		this->complexTransaction = transac;
	}
private:
	char pad_0x8[0x20]; //0x8
public:
	C_ComplexInventoryTransaction* complexTransaction; //0x28
};


class C_TextPacket : public C_Packet {
public:
	C_TextPacket() {
		static uintptr_t** textPacketVtable = 0x0;
		if (textPacketVtable == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 48 8B F9 48 89 01 48 83 C1 ?? E8 ?? ?? ?? ?? 48 8D 8F");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			textPacketVtable = reinterpret_cast<uintptr_t * *>(sigOffset + offset + /*length of instruction*/ 7);
			if (textPacketVtable == 0x0 || sigOffset == 0x0)
				logF("C_TextPacket signature not working!!!");
		}
		memset(this, 0, sizeof(C_TextPacket)); // Avoid overwriting vtable
		vTable = textPacketVtable;

		messageType = 1; // TYPE_CHAT
	}

	unsigned char gap0[0x20];
	unsigned __int8 messageType; // 0x28

	unsigned char gap[7]; 
	
	TextHolder sourceName;  // 0x30
	TextHolder message; // 0x50
	unsigned char field_24222[24]; // 0x70
	bool translationNeeded = false; // 0x88

	unsigned char ga2p[7]; // 0x89
	TextHolder xboxUserId;
	TextHolder platformChatId;
};

#pragma pack(push,8)

__declspec(align(8))
class C_MovePlayerPacket : public C_Packet {
public:
	C_MovePlayerPacket() {
		static uintptr_t** movePlayerPacketVtable = 0x0;
		if (movePlayerPacketVtable == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 01 48 8B 82 ?? ?? ?? ?? 48 89 41 ?? 48 8B 02 48 8B CA FF 50");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			movePlayerPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset  + offset + /*length of instruction*/ 7);
			if (movePlayerPacketVtable == 0x0 || sigOffset == 0x0)
				logF("C_MovePlayerPacket signature not working!!!");
		}
		memset(this, 0, sizeof(C_MovePlayerPacket)); // Avoid overwriting vtable
		vTable = movePlayerPacketVtable;
	}
	C_MovePlayerPacket(C_LocalPlayer* player,vec3_t pos) {
		static uintptr_t** movePlayerPacketVtable = 0x0;
		if (movePlayerPacketVtable == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 01 48 8B 82 ?? ?? ?? ?? 48 89 41 ?? 48 8B 02 48 8B CA FF 50");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			movePlayerPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (movePlayerPacketVtable == 0x0 || sigOffset == 0x0)
				logF("C_MovePlayerPacket signature not working!!!");
		}
		memset(this, 0, sizeof(C_MovePlayerPacket)); // Avoid overwriting vtable
		vTable = movePlayerPacketVtable;
		entityRuntimeID = player->entityRuntimeId;
		Position = pos;
		pitch = player->pitch;
		yaw = player->yaw;
		headYaw = player->yaw;
		onGround = true;
		mode = 0;
	}


	//uintptr_t** vTable;		// 0x0
private:
	char filler[0x20];		// 0x8
public:
	__int64 entityRuntimeID;//0x28
	vec3_t Position;		//0x30
	float pitch;			//0x3c
	float yaw;				//0x40
	float headYaw;			//0x44
	uint8_t mode;			//0x48
	bool onGround;			
	__int64 ridingEid;		
	int int1;				
	int int2;				
};

#pragma pack(pop)

