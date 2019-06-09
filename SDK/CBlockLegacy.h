#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "TextHolder.h"

class C_Material {
private:
	char pad[0x4];
public:
	bool isFlammable;
	bool isNeverBuildable;
	bool isAlwaysDestroyable;
	bool isReplaceable;
	bool isLiquid; // 0x0008
private:
	//char pad2[0x3]; // 0x009
public:
	float translucency; // 0x00C
	bool isBlockingMotion;
	bool isBlockingPrecipitation;
	bool isSolid;
	bool isSuperHot;
	float color[4];
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
	char pad_0x0080[0x35]; //0x0080
public:
	BYTE lightEmission; //0x00B5
private:
	char pad_0x00B6[0xE];//0x00B6
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

	inline C_BlockLegacy* toLegacy() { return *blockLegacy; }
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
