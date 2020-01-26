#pragma once

class C_GameSettingsInput {
private:
	char pad_0x0000[0x3E0];  //0x0000
public:
	int* spaceBarKey;  //0x3E0
private:
	char pad_0x370[0x38];  //0x3E8
public:
	int* sneakKey;  //0x420
private:
	char pad_0x0428[0x78];  //0x0428
public:
	int* leftKey;  //0x4A0
private:
	char pad_0x418[0x38];  //0x4A8
public:
	int* rightKey;  //0x4E0
private:
	char pad_0x450[0x38];  //0x4E8
public:
	int* backKey;  //0x520
private:
	char pad_0x488[0x38];  //0x528
public:
	int* forwardKey;  //0x560
};
