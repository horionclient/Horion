#pragma once

#include "../Utils/HMath.h"

class CEntityList
{
public:
private:
	char pad_0x0000[0x38]; //0x0000
public:
	uint64_t firstEntity; //0x0038 
	uint64_t lastEntity; //0x0040 
	uint64_t endAddr; //0x0048 


	uintptr_t getListSize() {
		return (lastEntity - firstEntity) / sizeof(uintptr_t);
	}
}; //Size=0x0050