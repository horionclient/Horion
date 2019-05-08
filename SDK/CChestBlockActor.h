#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "TextHolder.h"
#include "CEntityList.h"


class C_ChestBlockActor
{
public:
	uintptr_t** vTable; //0x0000
private:
	char pad_0x0008[0x24]; //0x0008 
private:
	vec3_ti Position; //0x002C

};
