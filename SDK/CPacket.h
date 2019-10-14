#pragma once
#include "../Utils/HMath.h"
#include "../Utils/Logger.h"
#include "TextHolder.h"
#include "CComplexInventoryTransaction.h"

class C_Packet {
public:
	uintptr_t** vTable; //0x0000
};


class C_ActorFallPacket : public C_Packet
{
private:
	char pad_0x8[0x18];//0x8
public:
	C_ActorFallPacket()
	{
		static uintptr_t** ActorFallPacketVtable = 0x0;
		if (ActorFallPacketVtable == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("48 8D 0D ?? ?? ?? ?? 48 89 4D CF 48 89 45 EF F3 44 0F 11 45 F7 88 5D FB");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			ActorFallPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (ActorFallPacketVtable == 0x0 || sigOffset == 0x0)
				logF("C_ActorFallPacketVtable signature not working!!!");
		}
		memset(this, 0, sizeof(C_ActorFallPacket)); // Avoid overwriting vtable
		vTable = ActorFallPacketVtable;
	}
	__int64 runtimeId;
	float fallDistance;
};

class C_MobEquipmentPacket : public C_Packet
{
public:
	C_MobEquipmentPacket()
	{
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
	C_MobEquipmentPacket(__int64 entityRuntimeId,C_ItemStack& item, int hotbarSlot, int inventorySlot)
	{
		memset(this, 0x0, sizeof(C_MobEquipmentPacket));
		using MobEquimentPacketConstructor_t = void(__fastcall*)(C_MobEquipmentPacket*,__int64, C_ItemStack&, int, int, char);
		static MobEquimentPacketConstructor_t MobEquimentPacketConstructor = reinterpret_cast<MobEquimentPacketConstructor_t>(Utils::FindSignature("48 89 4C 24 08 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 48 41 8B D9 48 8B F9 C7 41 ?? ?? ?? ?? ?? C7 41 ?? ?? ?? ?? ?? C6 41 ?? 00 48 ?? ?? ?? ?? ?? ?? ?? 48 8D 05 ?? ?? ?? ??"));
		if (MobEquimentPacketConstructor != 0)
			MobEquimentPacketConstructor(this, entityRuntimeId,item, hotbarSlot, inventorySlot, 0);
	}
private:
	char pad_0x8[0x18];//0x0
public:
	__int64 entityRuntimeId;//0x20
	C_ItemStack item;//0x28
	int inventorySlot; //0xB0
	int hotbarSlot;//0xB4
	char windowId;//0xB8
private:
	char unknown[0x2];//0xB9
public:
	char hotbarSlot1;//0xBA
	char inventorySlot1;//0xBB
private:
	char unknown1;//0xBC
};

class C_InventoryTransactionPacket : public C_Packet
{
public:
	C_InventoryTransactionPacket() {
		static uintptr_t** InventoryTransactionPacketVtable = 0x0;
		if (InventoryTransactionPacketVtable == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("48 8D 15 ?? ?? ?? ?? 49 89 53 C0 49 89 43 E0");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			InventoryTransactionPacketVtable = reinterpret_cast<uintptr_t * *>(sigOffset + offset + /*length of instruction*/ 7);
			if (InventoryTransactionPacketVtable == 0x0 || sigOffset == 0x0)
				logF("C_InventoryTransactionPacketVtable signature not working!!!");
		}
		memset(this, 0, sizeof(C_InventoryTransactionPacket)); // Avoid overwriting vtable
		vTable = InventoryTransactionPacketVtable;
	}
	C_InventoryTransactionPacket(C_ComplexInventoryTransaction* transac) {
		static uintptr_t** InventoryTransactionPacketVtable = 0x0;
		if (InventoryTransactionPacketVtable == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("48 8D 15 ?? ?? ?? ?? 49 89 53 C0 49 89 43 E0");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			InventoryTransactionPacketVtable = reinterpret_cast<uintptr_t * *>(sigOffset + offset + /*length of instruction*/ 7);
			if (InventoryTransactionPacketVtable == 0x0 || sigOffset == 0x0)
				logF("C_InventoryTransactionPacketVtable signature not working!!!");
		}
		memset(this, 0, sizeof(C_InventoryTransactionPacket)); // Avoid overwriting vtable
		vTable = InventoryTransactionPacketVtable;
		this->complexTransaction = transac;
	}
private:
	char pad_0x8[0x18]; //0x8
public:
	C_ComplexInventoryTransaction* complexTransaction; //0x20
};


class C_TextPacket : public C_Packet
{
public:
	C_TextPacket() {
		static uintptr_t** textPacketVtable = 0x0;
		if (textPacketVtable == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 85 ?? ?? ?? ?? C6 85 ?? ?? ?? ?? 01 4C");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			textPacketVtable = reinterpret_cast<uintptr_t * *>(sigOffset + offset + /*length of instruction*/ 7);
			if (textPacketVtable == 0x0 || sigOffset == 0x0)
				logF("C_TextPacket signature not working!!!");
		}
		memset(this, 0, sizeof(C_TextPacket)); // Avoid overwriting vtable
		vTable = textPacketVtable;

		messageType = 1; // TYPE_CHAT
	}

	unsigned char gap0[24];
	unsigned __int8 messageType;

	unsigned char gap[3];
	unsigned char field_24[4];
	
	TextHolder sourceName;
	TextHolder message;
	unsigned char field_24222[24];
	bool translationNeeded = false;

	unsigned char ga2p[7];
	TextHolder xboxUserId;
	TextHolder platformChatId;
};

#pragma pack(push,8)

__declspec(align(8))
class C_MovePlayerPacket : public C_Packet
{
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
	char filler[0x18];		// 0x8
public:
	__int64 entityRuntimeID;// 0x20
	vec3_t Position;		// 0x28
	float pitch;			// 0x34
	float yaw;				// 0x38
	float headYaw;			// 0x3C
	uint8_t mode;			// 0x40
	bool onGround;			// 0x41
	__int64 ridingEid;		// 0x48 // works because aligning
	int int1;				// 0x50
	int int2;				// 0x50
};

#pragma pack(pop)

