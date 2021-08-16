#pragma once

class C_MoveInputHandler {
private:
	char pad_0x0000[0x8]; //0x0000
public:
	float sideMovement; //0x0008
	float forwardMovement; //0x000C
private:
	char pad_0x0010[0x39]; //0x0010
public:
	unsigned char isSneakDown; //0x0049
private:
	char pad_0x004A[0x5]; //0x004A
public:
	unsigned char isJumping; //0x004F
	unsigned char autoJumpInWater; // 50

private:
	char pad_0x0051[0xE]; //0x0051
public:
	bool forward; //0x005F
	bool backward; //0x0060
	bool left; //0x0061
	bool right; //0x0062
private:
	char pad_0x0063[0x2D]; //0x0063
public:
	__int64 clearMovementState();;
};
