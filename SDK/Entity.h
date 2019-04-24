#pragma once

#include "../Utils/HMath.h"
#include "TextHolder.h"

class C_Entity
{
private:
	char pad_0x0000[0xD8]; //0x0000
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
	char pad_0x0104[0x48]; //0x0104
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
	uint64_t ticksAlive; //0x0258 
private:
	char pad_0x0260[0xA98]; //0x0260
protected:
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
	char pad_0x139C[0x8C]; //0x139C
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

class PointingStruct
{
public:
private:
	char pad_0x0000[0x570]; //0x0000
public:
	short rayHitType; //0x0570 
	short blockSide; //0x0574 
	vec3_ti block; //0x0578 
	vec3_t rayHitVec; //0x0584 
	uint64_t entityPtr; //0x0590 

}; //Size=0x0598