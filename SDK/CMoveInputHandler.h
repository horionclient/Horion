#pragma once

class C_MoveInputHandler {
private:
	char pad_0x0000[0xC];  //0x0000
public:
	float movingFriction;  //0xC
private:
	char pad_0x0010[0x66];  //0x10
public:
	bool isMovingForward;   //0x76
	bool isMovingBackward;  //0x77
	bool isMovingLeft;      //0x78
	bool isMovingRight;     //0x79
	__int64 ClearMovementState() {
		return Utils::CallVFunc<4, __int64>(this);
	};
};
