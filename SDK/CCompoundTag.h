#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "TextHolder.h"

#define EntityIdentifier "EntityIdentifier"
#define Delay "Delay"
#define MinSpawnDelay "MinSpawnDelay"
#define MaxSpawnDelay "MaxSpawnDelay"
#define SpawnCout "SpawnCount"
#define MaxNearbyEntities "MaxNearbyEntities"
#define RequiredPlayerRange "RequiredPlayerRange"
#define SpawnRange "SpawnRange"
#define DisplayEntityWidth "DisplayEntityWidth"
#define DisplayEntityHeight "DisplayEntityHeight"
#define DisplayEntityScale "DisplayEntityScale"

class Tag {
};

class ListTag : public Tag {
private:
	char pad_0x0[0x28];  //0x0000
};

class CompoundTag : public Tag {
private:
	char pad_0x0[0x18]; //0x0000

public:
	CompoundTag() {
		memset(this, 0, sizeof(CompoundTag));
		using constructor_t = void(__fastcall*)(CompoundTag*);
		constructor_t func = reinterpret_cast<constructor_t>(FindSignature("48 89 4C 24 ?? 57 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 48 8D 59"));
		func(this);
	}

	void putString(std::string tag, std::string value) {
		using putString_t = void(__fastcall*)(CompoundTag*, TextHolder&, TextHolder&);
		static putString_t func = reinterpret_cast<putString_t>(FindSignature("40 53 56 57 48 83 EC 50 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 49 8B F0 48 8B FA 48 89 54 24 ??"));
		func(this, TextHolder(tag), TextHolder(value));
	}

	void putShort(TextHolder& tag, short value) {
		using putShort_t = void(__fastcall*)(CompoundTag*, TextHolder&, short);
		static putShort_t func = reinterpret_cast<putShort_t>(FindSignature("4C 8B DC 55 56 57 48 83 EC ?? 49 ?? ?? ?? ?? ?? ?? ?? 49 89 5B 18 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? 89 44 24 ?? 41 0F B7 E8 48 8B F2 49 89 53 ??"));
		func(this, tag, value);
	}

	void putFloat(TextHolder& tag, float value) {
		using putFloat_t = void(__fastcall*)(CompoundTag*, TextHolder&, float);
		static putFloat_t func = reinterpret_cast<putFloat_t>(FindSignature("4C 8B DC 57 48 83 EC ?? 49 ?? ?? ?? ?? ?? ?? ?? 49 89 5B ?? 49 89 73 ?? 0F 29 74 24 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 0F 28 F2 48 8B F2 49 89 53 ??"));
		func(this, tag, value);
	}

	void put(TextHolder& tag, Tag** value) {
		using put_t = void(__fastcall*)(CompoundTag*, TextHolder&, Tag**);
		static put_t func = reinterpret_cast<put_t>(FindSignature("4C 8B DC 53 56 57 48 ?? ?? ?? ?? ?? ?? 49 ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 49 8B F8 48 8B DA 49 89 53 ?? 4D 89 43 ?? 49 8B 30 48 85 F6"));
		func(this, tag, value);
	}
};