#pragma once
#include "../Utils/HMath.h"
#include "../Utils/Logger.h"

class C_Packet {

};


#pragma pack(push,8)

__declspec(align(8))
class C_MovePlayerPacket : public C_Packet
{
public:
	C_MovePlayerPacket() {
		static uintptr_t** movePlayerPacketVtable = 0x0;
		if (movePlayerPacketVtable == 0x0) {
			uintptr_t sigOffset = Utils::FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 01 48 8B 82 ?? ?? ?? ?? 48 89 41 ?? 48 8B 02 48 8B CA FF 50 60");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			movePlayerPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset  + offset + /*length of instruction*/ 7);
			if (movePlayerPacketVtable == 0x0 || sigOffset == 0x0)
				logF("C_MovePlayerPacket signature not working!!!");
		}
		memset(this, 0, sizeof(C_MovePlayerPacket)); // Avoid overwriting vtable
		vTable = movePlayerPacketVtable;
	}


	uintptr_t** vTable;		// 0x0
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