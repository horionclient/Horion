#pragma once

#include "CClientInstance.h"
#include "CEntity.h"

#include <stdint.h>
#include "../Memory/MinHook.h"
#include "../Utils/HMath.h"

class C_GameMode {
private:
	uintptr_t* vtable;
public:
	C_Entity* player;

	void attack(C_Entity* entity) {
		using attack = void(__fastcall*)(void*, void*);
		static attack attackFunc = reinterpret_cast<attack>(Utils::FindSignature("48 8B C4 55 48 8D 68 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 ?? 48 89 70 ?? 48 89 78 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 48 8B FA 48 8B F1 48 C7 45 ?? 00 00 00 00 B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B D8 48 89 45 ?? 33 D2 41 B8 ?? ?? ?? ?? 48 8B C8 E8 ?? ?? ?? ?? 48 8B CB E8 ?? ?? ?? ?? 90 48 89 45 ?? C6"));

		if (attackFunc != 0x0)
			attackFunc(this,  entity);
	}

	void destroyBlock(vec3_ti* block, uint8_t blockSide) {
		using destroyBlock = void(__fastcall*)(void*, vec3_ti*, int);
		static destroyBlock destroyBlockFunc = reinterpret_cast<destroyBlock>(Utils::FindSignature("55 57 41 56 48 8D 68 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 ?? 48 89 70 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 45 0F B6 F0") - 3);

		if (destroyBlockFunc != 0x0) {
			destroyBlockFunc(this, block, blockSide);
		}
	}

	void buildBlock(vec3_ti* block, uint8_t blockSide) {
		using buildBlock_t = void(__fastcall*)(void*, vec3_ti*, int);
		static buildBlock_t buildBlock = reinterpret_cast<buildBlock_t>(Utils::FindSignature("48 8B C4 55 57 41 56  48 8D 68 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 ?? 48 89 70 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 41 0F B6 F0 48 8B FA"));

		if (buildBlock != 0x0) {
			buildBlock(this, block, blockSide);
		}
	}
};