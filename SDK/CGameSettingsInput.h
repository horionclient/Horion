#pragma once

class C_GameSettingsInput {
private:
	char pad_0x0000[0x460]; //0x0000
public:
	int* leftKey;//0x460
private:
	char pad_0x430[0x30];//0x468
public:
	int* rightKey;//0x498
private:
	char pad_0x468[0x30];//0x4A0
public:
	int* backKey;//0x4D0
private:
	char pad_0x4D4[0x30];//0x4D8
public:
	int* forwardKey;//0x508
};