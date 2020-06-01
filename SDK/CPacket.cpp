#include "CPacket.h"
#include "../Utils/Utils.h"

TextHolder* C_Packet::getName() {
	return Utils::CallVFunc<2, TextHolder*>(this, new TextHolder());
}
LevelSoundEventPacket::LevelSoundEventPacket() {
	static uintptr_t** LevelSoundEventPacketVtable = 0x0;
	if (LevelSoundEventPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 44 24 ?? 44 89 74 24 ?? F2 0F 10 06 F2 0F 11 44 24 ??");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		LevelSoundEventPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (LevelSoundEventPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(LevelSoundEventPacket));  // Avoid overwriting vtable
	vTable = LevelSoundEventPacketVtable;
	this->entityType.setText("minecraft:player");
}
PlayerAuthInputPacket::PlayerAuthInputPacket() {
	static uintptr_t** PlayerAuthInputPacketVtable = 0x0;
	if (PlayerAuthInputPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ?? ?? ?? ?? 48 89 11 48 89 79 ?? 48 89 79 ?? 89 79 ?? 48 89 79 ?? 48 89 79 ??");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		PlayerAuthInputPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (PlayerAuthInputPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(PlayerAuthInputPacket));  // Avoid overwriting vtable
	vTable = PlayerAuthInputPacketVtable;
}
PlayerAuthInputPacket::PlayerAuthInputPacket(__int64 entityRuntimeId, vec3_t pos, float pitch, float yaw, float yawUnused) {
	static uintptr_t** PlayerAuthInputPacketVtable = 0x0;
	if (PlayerAuthInputPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ?? ?? ?? ?? 48 89 11 48 89 79 ?? 48 89 79 ?? 89 79 ?? 48 89 79 ?? 48 89 79 ??");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		PlayerAuthInputPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (PlayerAuthInputPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(PlayerAuthInputPacket));  // Avoid overwriting vtable
	vTable = PlayerAuthInputPacketVtable;
	this->pos = pos;
	this->pitch = pitch;
	this->yaw = yaw;
	this->yawUnused = yawUnused;
	this->entityRuntimeId = entityRuntimeId;
}
C_ActorFallPacket::C_ActorFallPacket() {
	static uintptr_t** ActorFallPacketVtable = 0x0;
	if (ActorFallPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ?? ?? ?? ?? 48 89 4D ?? 48 89 45 EF F3 44 0F 11 45 F7 88 5D FB");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		ActorFallPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (ActorFallPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_ActorFallPacket));  // Avoid overwriting vtable
	vTable = ActorFallPacketVtable;
}
C_MobEquipmentPacket::C_MobEquipmentPacket() {
	static uintptr_t** MobEquipmentPacketVtable = 0x0;
	if (MobEquipmentPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 01 48 89 51 ?? 48 83 C1 28 49 8B D0");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		MobEquipmentPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (MobEquipmentPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_MobEquipmentPacket));  // Avoid overwriting vtable
	vTable = MobEquipmentPacketVtable;
}
C_MobEquipmentPacket::C_MobEquipmentPacket(__int64 entityRuntimeId, C_ItemStack& item, int hotbarSlot, int inventorySlot) {
	memset(this, 0x0, sizeof(C_MobEquipmentPacket));
	using MobEquimentPacketConstructor_t = void(__fastcall*)(C_MobEquipmentPacket*, __int64, C_ItemStack&, int, int, char);
	static MobEquimentPacketConstructor_t MobEquimentPacketConstructor = reinterpret_cast<MobEquimentPacketConstructor_t>(FindSignature("48 89 4C 24 08 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 48 41 8B D9 48 8B F9 C7 41 ?? ?? ?? ?? ?? C7 41 ?? ?? ?? ?? ?? C6 41 ?? 00 48 ?? ?? ?? ?? ?? ?? ?? 48 8D 05 ?? ?? ?? ??"));
	if (MobEquimentPacketConstructor != 0)
		MobEquimentPacketConstructor(this, entityRuntimeId, item, hotbarSlot, inventorySlot, 0);
}
C_InventoryTransactionPacket::C_InventoryTransactionPacket() {
	static uintptr_t** InventoryTransactionPacketVtable = 0x0;
	if (InventoryTransactionPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ?? ?? ?? ?? 49 89 53 C0 49 89 43");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		InventoryTransactionPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (InventoryTransactionPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_InventoryTransactionPacket));  // Avoid overwriting vtable
	vTable = InventoryTransactionPacketVtable;
}
C_TextPacket::C_TextPacket() {
	static uintptr_t** textPacketVtable = 0x0;
	if (textPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 8B F9 48 89 01 48 83 C1 ?? E8 ?? ?? ?? ?? 48 8D 8F");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		textPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (textPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_TextPacket));  // Avoid overwriting vtable
	vTable = textPacketVtable;

	messageType = 1;  // TYPE_CHAT
}
C_MovePlayerPacket::C_MovePlayerPacket() {
	static uintptr_t** movePlayerPacketVtable = 0x0;
	if (movePlayerPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 01 48 8B 82 ?? ?? ?? ?? 48 89 41 ?? 48 8B 02 48 8B CA FF 50");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		movePlayerPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (movePlayerPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_MovePlayerPacket));  // Avoid overwriting vtable
	vTable = movePlayerPacketVtable;
}
C_MovePlayerPacket::C_MovePlayerPacket(C_LocalPlayer* player, vec3_t pos) {
	static uintptr_t** movePlayerPacketVtable = 0x0;
	if (movePlayerPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 01 48 8B 82 ?? ?? ?? ?? 48 89 41 ?? 48 8B 02 48 8B CA FF 50");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		movePlayerPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (movePlayerPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_MovePlayerPacket));  // Avoid overwriting vtable
	vTable = movePlayerPacketVtable;
	entityRuntimeID = player->entityRuntimeId;
	Position = pos;
	pitch = player->pitch;
	yaw = player->yaw;
	headYaw = player->yaw;
	onGround = true;
	mode = 0;
}
C_PlayerActionPacket::C_PlayerActionPacket() {
	static uintptr_t** playerActionPacketVtable = 0x0;
	if (playerActionPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ?? ?? ?? ?? 48 89 4C 24 ?? 48 89 54 24 ?? 48 89 54 24 ?? 40 0F B6 CF");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		playerActionPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (playerActionPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_PlayerActionPacket));  // Avoid overwriting vtable
	vTable = playerActionPacketVtable;
}