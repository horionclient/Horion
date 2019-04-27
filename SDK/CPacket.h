#pragma once
#include "../Utils/HMath.h"

#pragma pack(push,8)

__declspec(align(8))
class MovePlayerPacket
{
public:
	uintptr_t** Vtable;		// 0x0
private:
	char filler[0x18];		// 0x8
public:
	__int64 entityRuntimeID;// 0x20
	vec3_t Position;		// 0x28
	vec3_t ViewAngles;		// 0x34
	uint8_t mode;			// 0x40
	bool onGround;			// 0x41
	__int64 ridingEid;		// 0x48 // works because aligning
	int int1;				// 0x50
	int int2;				// 0x50
};

#pragma pack(pop)