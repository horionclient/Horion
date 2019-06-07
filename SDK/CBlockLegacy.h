#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "TextHolder.h"

class C_Material {
private:
	char pad[0x7];
public:
	bool isReplaceable;
};

class C_BlockLegacy
{
public:
	uintptr_t** Vtable; //0x0000
	TextHolder tileName; // 0x0008
	TextHolder name; //0x0028
private:
	char pad_0x0048[0x30]; //0x0048
public:
	C_Material* material;
private:
	char pad_0x0080[0x44]; //0x0080
public:
	int blockId; // 0x00C4
	AABB aabb; //0x00C8
};

class C_Block
{
private:
	char pad[0x10];
public:
	C_BlockLegacy** blockLegacy;
};

class C_BlockSource
{
public:
	C_Block* getBlock(const vec3_ti& block) {
		using getBlock_t = C_Block*(__fastcall*)(C_BlockSource*, const vec3_ti&);
		static getBlock_t getBlock = reinterpret_cast<getBlock_t>(Utils::FindSignature("40 53 48 83 EC ?? 48 8B DA 8B 52 ?? 85 D2"));
		return getBlock(this, block);
	};
};
