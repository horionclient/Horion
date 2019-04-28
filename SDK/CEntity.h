#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "TextHolder.h"
#include "CEntityList.h"

class C_GameMode;

class PointingStruct
{
private:
	char pad_0x0000[0x570]; //0x0000
public:
	short rayHitType; //0x0570 
	short blockSide; //0x0574 
	vec3_ti block; //0x0578 
	vec3_t rayHitVec; //0x0584 
	uint64_t entityPtr; //0x0590 

}; //Size=0x0598

struct _ptr2 {
	char pad_0x0000[0x10]; //0x0000
	C_EntityList* entityList; //0x0010 
};

struct _ptr1 {
	char pad_0x0000[0x30]; //0x0000
	_ptr2* ptrToEntList; //0x0030 
};

struct _1ptr3 {
	char pad_0x0000[0x8B8];
	C_GameMode* cGameMode; //0x08B8
};

struct _1ptr2 {
	char pad_0x0000[0x18]; //0x0000
	_1ptr3* ptrToCGameMode; //0x0018 
};

struct _1ptr1 {
	char pad_0x0000[0x238]; //0x0000
	_1ptr2* ptrToPtrToCGameMode; //0x0238 
};

class C_Entity
{
public:
	uintptr_t** vTable; //0x0000
	_ptr1* ptrToPtrToEntList; //0x0008 
private:
	char pad_0x0010[0xD0]; //0x0010
public:
	float pitch; //0x00E0
	float yaw; //0x00E4
	float pitch2; //0x00E8
	float yaw2; //0x00EC
private:
	char pad_0x00F0[0x10]; //0x00F0
public:
	vec3_t eyePos0; //0x0100 
private:
	char pad_0x010C[0xC]; //0x010C
public:
	_1ptr1* ptrToPtrToPtrToCGameMode; //0x0118
private:
	char pad_0x0120[0x34]; //0x0120
public:
	uint8_t onGround; //0x0154
private:
	char pad_0x0155[0x63]; //0x0155
public:
	int entityType1; //0x01B8 
private:
	char pad_0x01BC[0x33]; //0x01BC
public:
	char entityType2; //0x01EF
private:
	char pad_0x01F0[0x8]; //0x01F0
public:
	float stepHeight; //0x01F8
private:
	char pad_0x01FC[0x64]; //0x01FC
public:
	uintptr_t ticksAlive; //0x0260 
private:
	char pad_0x0268[0xA98]; //0x0268
public:
	PointingStruct* pointingAt; //0x0D00
private:
	char pad_0x0D08[0x2A8]; //0x0D08
public:
	AABB aabb; //0x0FB0 
private:
	char pad_0x0FC8[0x4]; //0x0FC8
public:
	float width; //0x0FCC
	float height; //0x0FD0 
	vec3_t velocity; //0x0FD4
	vec3_t eyePos1; //0x0FE0
	vec3_t eyePos2; //0x0FEC
private:
	char pad_0x0FF0[0x58]; //0x0FF8
public:
	__int64 entityRuntimeId; //0x1050
private:
	char pad_0x1050[0x30]; //0x1058
public:
	TextHolder name; //0x1088
private:
	char pad_0x109C[0x2F4]; //0x109C
public:
	float bodyYaw; //0x1390
	float oldBodyYaw; //0x1394
	float yawUnused1; //0x1398
	float yawUnused2; //0x139C
	int damageTime; //0x13A0 
private:
	char pad_0x13A4[0x70]; //0x13A4
public:
	int swing; //0x1414 
private:
	char pad_0x1418[0x18]; //0x1418
public:
	int timeSinceDeath; //0x1430
private:
	char pad_0x1434[0x604]; //0x1434
public:
	uint16_t itemData;//0x1A38 
	uint16_t itemId;
private:
	char pad_0x1A3C[0x20C]; //0x1A3C
public:
	int gamemode; //0x1C48 
private:
	char pad_0x1C4C[0x60]; //0x1C4C
public:
	TextHolder uuid; //0x1CAC 
};

class C_LocalPlayer : public C_Entity {
public:
	C_EntityList* getEntityList() {
		return this->ptrToPtrToEntList->ptrToEntList->entityList;
	}

	C_GameMode* getCGameMode() {
		return this->ptrToPtrToPtrToCGameMode->ptrToPtrToCGameMode->ptrToCGameMode->cGameMode;
	}

	void displayClientMessageStr(const std::string str) { // vtable[405]
		using displayClientMessage = void(__fastcall*)(void*, const std::string*);
		//static displayClientMessage displayMessageFunc = reinterpret_cast<displayClientMessage>(Utils::FindSignature("40 53 48 83 EC 20 48 8B 89 ?? ?? ?? ?? 48 8B DA 48 8B 01 FF 90 ?? ?? ?? ?? 48 8B C8"));
		static displayClientMessage displayMessageFunc = reinterpret_cast<displayClientMessage>(this->vTable[405]);
		displayMessageFunc(this, &str);
	}

	void displayClientMessage(char* message) {
		//displayClientMessageStr(std::wstring(message));
	}

	void swingArm() {
		using SwingArm = void(__thiscall*)(void*);
		static SwingArm swingFunc = reinterpret_cast<SwingArm>(Utils::FindSignature("40 57 48 83 EC ?? 48 C7 44 24 ?? FE FF FF FF 48 89 5C 24 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B D9 80 B9"));
		swingFunc(this);
	}
};