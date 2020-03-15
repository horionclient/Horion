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
#define TicksLeftToStay "TicksLeftToStay"
#define SaveData "SaveData"

class C_Tag {
public:
	uintptr_t** vTable;
};

class C_CompoundTag;

class C_ListTag : public C_Tag {
private:
	char pad_0x0[0x20];  //0x0008
public:
	C_ListTag() {
		static uintptr_t** ListTagVtable = 0x0;
		if (ListTagVtable == 0x0) {
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 45 00 4D 89 65 ?? 4D 89 65 ?? 4D 89 65 ?? 45 88 65 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 44 89 7C 24");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			ListTagVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (ListTagVtable == 0x0 || sigOffset == 0x0)
				logF("ListTagVtable signature not working!!!");
		}
		memset(this, 0, sizeof(C_ListTag));  // Avoid overwriting vtable
		vTable = ListTagVtable;
	}
	void add(std::unique_ptr<C_Tag> tag) {
		using ListTag__add_t = void(__fastcall*)(C_ListTag*, std::unique_ptr<C_Tag>);
		ListTag__add_t func = reinterpret_cast<ListTag__add_t>(FindSignature("40 57 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B FA 48 8B D9 48 89 54 24 ?? 48 8B 0A 48 8B 01 FF 50"));
		func(this, std::move(tag));
	}
};

class C_CompoundTag : public C_Tag {
private:
	char pad_0x0[0x10];  //0x0008

public:
	C_CompoundTag() {
		memset(this, 0, sizeof(C_CompoundTag));
		using constructor_t = void(__fastcall*)(C_CompoundTag*);
		constructor_t func = reinterpret_cast<constructor_t>(FindSignature("48 89 4C 24 ?? 57 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 48 8D 59"));
		func(this);
	}

	void putString(std::string tag, std::string value) {
		using putString_t = void(__fastcall*)(C_CompoundTag*, TextHolder&, TextHolder&);
		static putString_t func = reinterpret_cast<putString_t>(FindSignature("40 53 56 57 48 83 EC 50 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 49 8B F0 48 8B FA 48 89 54 24 ??"));
		func(this, TextHolder(tag), TextHolder(value));
	}

	void putShort(TextHolder& tag, short value) {
		using putShort_t = void(__fastcall*)(C_CompoundTag*, TextHolder&, short);
		static putShort_t func = reinterpret_cast<putShort_t>(FindSignature("4C 8B DC 55 56 57 48 83 EC ?? 49 ?? ?? ?? ?? ?? ?? ?? 49 89 5B 18 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? 89 44 24 ?? 41 0F B7 E8 48 8B F2 49 89 53 ??"));
		func(this, tag, value);
	}

	void putFloat(TextHolder& tag, float value) {
		using putFloat_t = void(__fastcall*)(C_CompoundTag*, TextHolder&, float);
		static putFloat_t func = reinterpret_cast<putFloat_t>(FindSignature("4C 8B DC 57 48 83 EC ?? 49 ?? ?? ?? ?? ?? ?? ?? 49 89 5B ?? 49 89 73 ?? 0F 29 74 24 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 0F 28 F2 48 8B F2 49 89 53 ??"));
		func(this, tag, value);
	}

	void put(TextHolder& tag, std::unique_ptr<C_Tag> value) {
		using put_t = void(__fastcall*)(C_CompoundTag*, TextHolder&, std::unique_ptr<C_Tag>);
		static put_t func = reinterpret_cast<put_t>(FindSignature("4C 8B DC 53 56 57 48 ?? ?? ?? ?? ?? ?? 49 ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 49 8B F8 48 8B DA 49 89 53 ?? 4D 89 43 ?? 49 8B 30 48 85 F6"));
		func(this, tag, std::move(value));
	}

	void putInt(TextHolder& text, int value) {
		using putInt_t = void(__fastcall*)(C_CompoundTag*, TextHolder&, int);
		static putInt_t func = reinterpret_cast<putInt_t>(FindSignature("4C 8B DC 55 56 57 48 83 EC ?? 49 ?? ?? ?? ?? ?? ?? ?? 49 89 5B 18 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 41 8B E8 48 8B F2 49 89 53 ??"));
		func(this, text, value);
	}

	void putCompound(TextHolder& text, std::unique_ptr<C_CompoundTag> tag) {
		using putCompound_t = void(__fastcall*)(C_CompoundTag*, TextHolder&, std::unique_ptr<C_CompoundTag>);
		static putCompound_t func = reinterpret_cast<putCompound_t>(FindSignature("40 53 56 57 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 49 8B D8 48 8B FA 48 89 54 24 ?? 48 89 5C ?? 40 49 83 38 ?? 74 4A"));
		func(this, text, std::move(tag));
	}

	std::unique_ptr<C_CompoundTag> clone(std::unique_ptr<C_CompoundTag> tag) {
		using clone_t = std::unique_ptr<C_CompoundTag>(__fastcall*)(C_CompoundTag*,std::unique_ptr<C_CompoundTag>);
		static clone_t func = reinterpret_cast<clone_t>(FindSignature("48 89 54 24 ?? 57 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 8B FA 48 8B D9 C7 44 ?? ?? ?? ?? ?? ?? 33 C0 48 89 02 48 8B CA"));
		return func(this, std::move(tag));
	}
};