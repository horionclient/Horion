#pragma once

class C_GameSettingsInput {
private:
	char pad_0x0000[0x368];//0x0000
public:
	int* spaceBarKey;//0x368
private:
	char pad_0x370[0xA0];//0x370
public:
	int* leftKey;//0x410
private:
	char pad_0x418[0x30];//0x418
public:
	int* rightKey;//0x448
private:
	char pad_0x450[0x30];//0x450
public:
	int* backKey;//0x480
private:
	char pad_0x488[0x30];//0x488
public:
	int* forwardKey;//0x4B8
};