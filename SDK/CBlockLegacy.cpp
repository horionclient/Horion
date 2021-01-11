#include "CBlockLegacy.h"

#include "../Memory/GameData.h"
#include "../Utils/Utils.h"
C_Block* C_BlockSource::getBlock(const vec3_ti& block) {
	using getBlock_t = C_Block*(__fastcall*)(C_BlockSource*, const vec3_ti&);
	static getBlock_t getBlock = reinterpret_cast<getBlock_t>(FindSignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 48 8B DA 8B 4A"));
	return getBlock(this, block);
}
C_BlockActor* C_BlockSource::getBlockEntity(const vec3_ti& block) {
	using getBlockEntity_t = C_BlockActor*(__fastcall*)(C_BlockSource*, const vec3_ti&);
	static getBlockEntity_t getBlockEntity = reinterpret_cast<getBlockEntity_t>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 8B 02 48 8B DA C1 F8 ? 48 8B F1 89 44 24 ? 8B 42"));
	return getBlockEntity(this, block);
}
C_Block* C_BlockSource::getLiquidBlock(const vec3_ti& block) {
	using getLiquidBlock_t = C_Block*(__fastcall*)(C_BlockSource*, const vec3_ti&);
	static getLiquidBlock_t getLiquidBlock = reinterpret_cast<getLiquidBlock_t>(FindSignature("48 89 5C 24 08 57 48 83 EC ?? 83 7A ?? 00 48 8B DA 48 8B F9 7C"));
	return getLiquidBlock(this, block);
}
bool C_BlockLegacy::getCollisionShape(AABB* collShapeOut, C_Block* block, C_BlockSource* blockSource, const vec3_ti* pos, C_Entity* actor) {
	return Utils::CallVFunc<5, bool, AABB*, C_Block*, C_BlockSource*, const vec3_ti*, C_Entity*>(this, collShapeOut, block, blockSource, pos, actor);
}
int C_BlockLegacy::liquidGetDepth(C_BlockSource* reg, const vec3_ti* pos) {
	using liquid_getDepth_t = int(__fastcall*)(C_BlockLegacy*, C_BlockSource*, const vec3_ti*);
	static liquid_getDepth_t liquid_getDepth = reinterpret_cast<liquid_getDepth_t>(FindSignature("40 53 48 83 EC ?? 48 8B C2 48 8B D9 48 8B C8 49 8B D0 E8 ?? ?? ?? ?? 48 8B 8B"));
	return liquid_getDepth(this, reg, pos);
}
void C_BlockLegacy::liquidGetFlow(vec3_t* flowOut, C_BlockSource* reg, const vec3_ti* pos) {
	using liquid_getFlow_t = int(__fastcall*)(C_BlockLegacy*, vec3_t*, C_BlockSource*, const vec3_ti*);
	static liquid_getFlow_t liquid_getDepth = nullptr;
	if(liquid_getDepth == nullptr){
		if(g_Data.getVersion() == GAMEVERSION::g_1_16_0)
			liquid_getDepth = reinterpret_cast<liquid_getFlow_t>(FindSignature("48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 C7 45 ? ? ? ? ? 48 89 58 ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B F1 4C 89 4C 24"));
		else
			liquid_getDepth = reinterpret_cast<liquid_getFlow_t>(FindSignature("40 55  56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? ?? ?? ?? ?? 48 89 9C 24 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 49 8B F1 4C 89 4C"));
	}
	liquid_getDepth(this, flowOut, reg, pos);
}
bool C_BlockLegacy::hasWater(C_BlockSource* reg, const vec3_ti& pos) {
	if(this->material->isLiquid){
		return !this->material->isSuperHot;
	}

	auto liquidBlock = reg->getLiquidBlock(pos)->toLegacy();
	return this != liquidBlock && liquidBlock->material->isLiquid && !liquidBlock->material->isSuperHot;
}
