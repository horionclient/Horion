#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "TextHolder.h"
#include "CEntityList.h"


class C_ChestBlockActor {
public:
	uintptr_t** vTable; //0x0000
private:
	char pad_0x0008[0x8]; //0x0008 
public:
	uintptr_t* block;  //0x0010 
private:
	char pad_0x0018[0x14]; //0x0018
public:
	vec3_ti posI; //0x002C
	AABB aabb;
};
