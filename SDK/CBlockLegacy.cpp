#include "CBlockLegacy.h"

#include "../Utils/Utils.h"
C_Block* C_BlockSource::getBlock(const vec3_ti& block) {
	using getBlock_t = C_Block*(__fastcall*)(C_BlockSource*, const vec3_ti&);
	static getBlock_t getBlock = reinterpret_cast<getBlock_t>(FindSignature("40 53 48 83 EC ?? 48 8B DA 8B 52 ?? 85 D2"));
	return getBlock(this, block);
}
C_BlockActor* C_BlockSource::getBlockEntity(const vec3_ti& block) {
	using getBlockEntity_t = C_BlockActor*(__fastcall*)(C_BlockSource*, const vec3_ti&);
	static getBlockEntity_t getBlockEntity = reinterpret_cast<getBlockEntity_t>(FindSignature("40 53 48 83 EC ?? 8B 02 48 8B DA C1 F8 ?? 89 44 24 ?? 8B 42 ?? 48 8D 54 24 ? C1 F8 04 89 44 24 ?? E8 ? ? ? ? 48 85 C0 74 31"));
	return getBlockEntity(this, block);
}
