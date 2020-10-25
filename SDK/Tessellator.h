#pragma once

#include "../Utils/HMath.h"

class Tessellator {
private:
	char pad_0000[0x128];
	vec3_t transformOffset; // 0x128

public:
	void addPostTransformOffset(int x, int y, int z) {
		addPostTransformOffset((float)x, (float)y, (float)z);
	}
	void addPostTransformOffset(float x, float y, float z);
};

class C_Block;

class BlockTessellator {
public:
	__int64 getMeshForBlockInWorld(Tessellator * t, C_Block * block, vec3_ti & pos);
};