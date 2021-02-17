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
	char pad2[0x3]; // 0x009
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
	uintptr_t** Vtable;         //0x0000
	class TextHolder tileName;  //0x0008
private:
	char pad_0028[8];  //0x0028
public:
	class TextHolder name;  //0x0030
private:
	char pad_0050[136];  //0x0050
public:
	class C_Material* material;  //0x00D8
private:
	char pad_00E0[104];  //0x00E0
public:
	int64_t blockId;  //0x0148

	int liquidGetDepth(C_BlockSource*, const vec3_ti* pos);
	void liquidGetFlow(vec3_t* flowOut, C_BlockSource*, const vec3_ti* pos);
	bool getCollisionShape(AABB* collShapeOut, C_Block* block, C_BlockSource* blockSource, const vec3_ti* pos, C_Entity* actor);
	bool hasWater(C_BlockSource*, const vec3_ti& pos);
};

class C_Block {
public:
	uint8_t data; // 0x8

private:
	char pad[0x7];

public:
	C_BlockLegacy* blockLegacy; // 0x10

	inline C_BlockLegacy* toLegacy() { return blockLegacy; }

	virtual ~C_Block();
	virtual int getRenderLayer();
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
