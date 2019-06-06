#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "TextHolder.h"

class C_BlockLegacy
{
public:
	uintptr_t** Vtable; //0x0000
private:
	char pad_0x0008[0x20]; //0x0008
public:
	char name[20]; //0x0028
private:
	char pad_0x0048[0x80]; //0x0048
public:
	AABB aabb; //0x00C8
};

