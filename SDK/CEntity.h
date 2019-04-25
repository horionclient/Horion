#pragma once

#include "../Utils/HMath.h"
#include "TextHolder.h"
#include "CEntityList.h"
#include "CGameMode.h"

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
private:
	char pad_0x0000[0x8]; //0x0000
public:
	_ptr1* ptrToPtrToEntList; //0x0008 
private:
	char pad_0x0010[0xC8]; //0x0010
public:
	float pitch; //0x00D8 
	float yaw; //0x00DC 
	float pitch2; //0x00E0 
	float yaw2; //0x00E4 
private:
	char pad_0x00E8[0x10]; //0x00E8
public:
	vec3_t eyePos0; //0x00F8 
private:
	char pad_0x0104[0xC]; //0x0104
public:
	_1ptr1* ptrToPtrToPtrToCGameMode; //0x0110 
private:
	char pad_0x0118[0x34]; //0x0118
public:
	uint8_t onGround; //0x014C 
private:
	char pad_0x014D[0x63]; //0x014D
public:
	int entityType1; //0x01B0 
private:
	char pad_0x01B4[0x33]; //0x01B4
public:
	char entityType2; //0x01E7 
private:
	char pad_0x01E8[0x8]; //0x01E8
public:
	float stepHeight; //0x01F0 
private:
	char pad_0x01F4[0x64]; //0x01F4
public:
	uintptr_t ticksAlive; //0x0258 
private:
	char pad_0x0260[0xA98]; //0x0260
public:
	PointingStruct* pointingAt; //0x0CF8 
private:
	char pad_0x0D00[0x2A8]; //0x0D00
public:
	AABB aabb; //0x0FA8 
private:
	char pad_0x0FC0[0x4]; //0x0FC0
public:
	float width; //0x0FC4 
	float height; //0x0FC8 
	vec3_t velocity; //0x0FCC 
	vec3_t eyePos1; //0x0FD8 
	vec3_t eyePos2; //0x0FE4 
private:
	char pad_0x0FF0[0x90]; //0x0FF0
public:
	TextHolder name; //0x1080 
private:
	char pad_0x1094[0x2F4]; //0x1094
public:
	float bodyYaw; //0x1388 
	float oldBodyYaw; //0x138C 
	float yawUnused1; //0x1390 
	float yawUnused2; //0x1394 
	int damageTime; //0x1398 
private:
	char pad_0x139C[0x70]; //0x139C
public:
	int swing; //0x140C 
private:
	char pad_0x1410[0x18]; //0x1410
public:
	int timeSinceDeath; //0x1428 
private:
	char pad_0x142C[0x604]; //0x142C
public:
	uint16_t itemData;//0x1A30 
	uint16_t itemId;
private:
	char pad_0x1A34[0x20C]; //0x1A34
public:
	int gamemode; //0x1C40 
private:
	char pad_0x1C44[0x60]; //0x1C44
public:
	TextHolder uuid; //0x1CA4 
};

class C_LocalPlayer : public C_Entity {
public:
	C_EntityList* getEntityList() {
		return this->ptrToPtrToEntList->ptrToEntList->entityList;
	}

	C_GameMode* getCGameMode() {
		return this->ptrToPtrToPtrToCGameMode->ptrToPtrToCGameMode->ptrToCGameMode->cGameMode;
	}
};