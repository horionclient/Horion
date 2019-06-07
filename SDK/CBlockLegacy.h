#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "TextHolder.h"

class C_BlockLegacy
{
public:
	uintptr_t** Vtable; //0x0000
	TextHolder tileName; // 0x0008
	TextHolder name; //0x0028
private:
	char pad_0x0048[0x80]; //0x0048
public:
	AABB aabb; //0x00C8
};

