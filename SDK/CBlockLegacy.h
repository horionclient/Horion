#pragma once

#include "../Utils/HMath.h"
#include "Tag.h"


class C_Material {
private:
	char pad[0x4];

public:
	bool isFlammable;
	bool isNeverBuildable;
	bool isAlwaysDestroyable;
	bool isReplaceable;
	bool isLiquid;  // 0x0008
private:
	//char pad2[0x3]; // 0x009
public:
	float translucency;  // 0x00C
	bool isBlockingMotion;
	bool isBlockingPrecipitation;
	bool isSolid;
	bool isSuperHot;
	float color[4];
};

class C_BlockLegacy {
public:
	uintptr_t** Vtable;   //0x0000
	TextHolder tileName;  // 0x0008
	TextHolder name;      //0x0028
private:
	char pad_0x0048[0x70];  //0x0048
public:
	C_Material* material;  //0x00B8
private:
	char pad_0x00C0[0x44];  //0x00C0
public:
	short blockId;  // 0x00C4
private:
	char pad_0x0106[0xA];  //0x0106
public:
	AABB aabb;  //0x00C8
};

class C_Block {
private:
	uintptr_t** vtable;

public:
	uint8_t data;

private:
	char pad[0x7];

public:
	C_BlockLegacy** blockLegacy;

	inline C_BlockLegacy* toLegacy() { return *blockLegacy; }
};

class C_BlockActor {
private:
	virtual void destructor();
	virtual __int64 load(__int64&, CompoundTag*, __int64&);

public:
	virtual bool save(CompoundTag*);
};

class C_BlockSource {
public:
	C_Block* getBlock(const vec3_ti& block) {
		using getBlock_t = C_Block*(__fastcall*)(C_BlockSource*, const vec3_ti&);
		static getBlock_t getBlock = reinterpret_cast<getBlock_t>(FindSignature("40 53 48 83 EC ?? 48 8B DA 8B 52 ?? 85 D2"));
		return getBlock(this, block);
	};

	C_BlockActor* getBlockEntity(const vec3_ti& block) {
		using getBlockEntity_t = C_BlockActor*(__fastcall*)(C_BlockSource*, const vec3_ti&);
		static getBlockEntity_t getBlockEntity = reinterpret_cast<getBlockEntity_t>(FindSignature("40 53 48 83 EC ?? 8B 02 48 8B DA C1 F8 ?? 89 44 24 ?? 8B 42 ?? 48 8D 54 24 ? C1 F8 04 89 44 24 ?? E8 ? ? ? ? 48 85 C0 74 31"));
		return getBlockEntity(this, block);
	}
};
