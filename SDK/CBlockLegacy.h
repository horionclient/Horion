#pragma once

#include "../Utils/HMath.h"
#include "TextHolder.h"
//#include "Tag.h"

class C_Material {
public:
	int type; // 5 for water, 6 for lava
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

class C_Entity;
class C_Block;
class C_BlockSource;

class C_BlockLegacy {
public:
	uintptr_t** Vtable;   //0x0000
	TextHolder tileName;  // 0x0008
	TextHolder name;      //0x0028
private:
	char pad_0x0048[0x78];  //0x0048
public:
	C_Material* material;  //0x00C0
private:
	char pad_0x00C0[0x44];  //0x00C8
public:
	short blockId;  // 0x010C
private:
	char pad_0x0106[0xA];  //0x010E
public:
	AABB aabb;  //0x0110

	int liquidGetDepth(C_BlockSource*, const vec3_ti* pos);
	void liquidGetFlow(vec3_t* flowOut, C_BlockSource*, const vec3_ti* pos);
	bool getCollisionShape(AABB* collShapeOut, C_Block* block, C_BlockSource* blockSource, const vec3_ti* pos, C_Entity* actor);
	bool hasWater(C_BlockSource*, const vec3_ti& pos);
};

class C_Block {
private:
	uintptr_t** vtable;

public:
	uint8_t data; // 0x8

private:
	char pad[0x7];

public:
	C_BlockLegacy** blockLegacy; // 0x10

	inline C_BlockLegacy* toLegacy() { return *blockLegacy; }
};

class CompoundTag;

class C_BlockActor {
private:
	virtual void destructor();
	virtual __int64 load(__int64&, CompoundTag*, __int64&);

public:
	virtual bool save(CompoundTag*);
};

class C_BlockSource {
public:
	C_Block* getBlock(const vec3_ti& block);;

	C_BlockActor* getBlockEntity(const vec3_ti& block);

	C_Block* getLiquidBlock(const vec3_ti& block);
};
