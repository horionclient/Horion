#include "Tag.h"

#include "../Utils/Utils.h"
#include "../Utils/Logger.h"
#include "../Memory/GameData.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

Int64Tag::Int64Tag(__int64 value) {
	static uintptr_t** Int64TagVtable = 0x0;
	if (Int64TagVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 47 40 48 89 77 48 C6 47 68 04 48 8B CD E8 ? ? ? ?");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		Int64TagVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
		if (Int64TagVtable == 0x0 || sigOffset == 0x0)
			logF("Int64TagVtable signature not working!!!");
	}
	memset(this, 0, sizeof(Int64Tag));  // Avoid overwriting vtable
	uintptr_t* _this = reinterpret_cast<uintptr_t*>(this);
	*_this = (uintptr_t)Int64TagVtable;
	this->value = value;
}
void Int64Tag::read(std::string& string)  //throws MojangsonParseException
{
	std::transform(string.begin(), string.end(), string.begin(), ::tolower);
	char lastChar = string.at(string.length() - 1);
	if (lastChar == MojangsonToken::LONG_SUFFIX.getSymbol()) {
		string = string.substr(0, string.length() - 1);
	}

	try {
		value = std::stoull(string);
	} catch (...) {
		logF(" Unhandled exception for Int64Tag : %s", string.c_str());
	}
}
IntTag::IntTag(int value) {
	static uintptr_t** IntTagVtable = 0x0;
	if (IntTagVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ? ? ? ? 48 89 10 89 48 08 48 89 03 48 8B C3 48 8B 5C 24 ?");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		IntTagVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
		if (IntTagVtable == 0x0 || sigOffset == 0x0)
			logF("IntTagVtable signature not working!!!");
	}
	memset(this, 0, sizeof(IntTag));  // Avoid overwriting vtable
	uintptr_t* _this = reinterpret_cast<uintptr_t*>(this);
	*_this = (uintptr_t)IntTagVtable;
	this->value = value;
}
void IntTag::read(std::string& string)  //throws MojangsonParseException
{
	try {
		value = static_cast<int>(std::stoi(string));
	} catch (...) {
		logF(" Unhandled exception for IntTag : %s", string.c_str());
	}
}
StringTag::StringTag(std::string value) {
	static uintptr_t** StringTagVtable = 0x0;
	if (StringTagVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 03 0F 10 44 24 ? 0F 11 43 ? 0F 10 4C 24 ? 0F 11 4B");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		StringTagVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
		if (StringTagVtable == 0x0 || sigOffset == 0x0)
			logF("StringTagVtable signature not working!!!");
	}
	memset(this, 0, sizeof(StringTag));  // Avoid overwriting vtable
	uintptr_t* _this = reinterpret_cast<uintptr_t*>(this);
	*_this = (uintptr_t)StringTagVtable;
	this->value = value;
}
ShortTag::ShortTag(short value) {
	static uintptr_t** ShortTagVtable = 0x0;
	if (ShortTagVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ? ? ? ? 48 89 10 66 89 48 ? 48 89 03 48 8B C3 48 8B 5C 24 ?");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		ShortTagVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
		if (ShortTagVtable == 0x0 || sigOffset == 0x0)
			logF("ShortTagVtable signature not working!!!");
	}
	memset(this, 0, sizeof(ShortTag));  // Avoid overwriting vtable
	uintptr_t* _this = reinterpret_cast<uintptr_t*>(this);
	*_this = (uintptr_t)ShortTagVtable;
	this->value = value;
}
void ShortTag::read(std::string& string)  //throws MojangsonParseException
{
	std::transform(string.begin(), string.end(), string.begin(), ::tolower);
	char lastChar = string.at(string.length() - 1);
	if (lastChar == MojangsonToken::SHORT_SUFFIX.getSymbol()) {
		string = string.substr(0, string.length() - 1);
	}

	try {
		value = static_cast<short>(std::stoi(string));
	} catch (...) {
		logF(" Unhandled exception for ShortTag : %s", string.c_str());
	}
}
FloatTag::FloatTag(float value) {
	static uintptr_t** FloatTagVtable = 0x0;
	if (FloatTagVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 06 8B 47 08 89 46 08 48 8B 03 C6 40 28 05");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		FloatTagVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
		if (FloatTagVtable == 0x0 || sigOffset == 0x0)
			logF("FloatTagVtable signature not working!!!");
	}
	memset(this, 0, sizeof(FloatTag));  // Avoid overwriting vtable
	uintptr_t* _this = reinterpret_cast<uintptr_t*>(this);
	*_this = (uintptr_t)FloatTagVtable;
	this->value = value;
}
void FloatTag::read(std::string& string)  // MojangsonParseException
{
	std::transform(string.begin(), string.end(), string.begin(), ::tolower);
	char lastChar = string.at(string.length() - 1);
	if (lastChar == MojangsonToken::FLOAT_SUFFIX.getSymbol()) {
		string = string.substr(0, string.length() - 1);
	}

	try {
		value = static_cast<float>(std::stof(string));
	} catch (...) {
		logF(" Unhandled exception for FloatTag : %s", string.c_str());
	}
}
DoubleTag::DoubleTag(double value) {
	static uintptr_t** DoubleTagVtable = 0x0;
	if (DoubleTagVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? F2 0F 11 40 ? 48 89 08 48 89 03 48 8B C3 48 8B 5C 24 ? 48 83 C4 30");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		DoubleTagVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
		if (DoubleTagVtable == 0x0 || sigOffset == 0x0)
			logF("DoubleTagVtable signature not working!!!");
	}
	memset(this, 0, sizeof(DoubleTag));  // Avoid overwriting vtable
	uintptr_t* _this = reinterpret_cast<uintptr_t*>(this);
	*_this = (uintptr_t)DoubleTagVtable;
	this->value = value;
}
void DoubleTag::read(std::string& string)  //throws MojangsonParseException
{
	char lastChar = string.at(string.length() - 1);
	if (lastChar == MojangsonToken::DOUBLE_SUFFIX.getSymbol()) {
		string = string.substr(0, string.length() - 1);
	}

	try {
		value = static_cast<double>(std::stod(string));
	} catch (...) {
		logF(" Unhandled exception for DoubleTag : %s", string.c_str());
	}
}
ByteTag::ByteTag(char value) {
	static uintptr_t** ByteTagVtable = 0x0;
	if (ByteTagVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ? ? ? ? 48 89 10 88 48 ? 48 89 03 48 8B C3 48 8B 5C 24 ?");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		ByteTagVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
		if (ByteTagVtable == 0x0 || sigOffset == 0x0)
			logF("ByteTagVtable signature not working!!!");
	}
	memset(this, 0, sizeof(ByteTag));  // Avoid overwriting vtable
	uintptr_t* _this = reinterpret_cast<uintptr_t*>(this);
	*_this = (uintptr_t)ByteTagVtable;
	this->value = value;
}
void ByteTag::read(std::string& string)  //throws MojangsonParseException
{
	std::transform(string.begin(), string.end(), string.begin(), ::tolower);
	char lastChar = string.at(string.length() - 1);
	if (lastChar == MojangsonToken::BYTE_SUFFIX.getSymbol()) {
		string = string.substr(0, string.length() - 1);
	}

	try {
		value = static_cast<char>(std::stoi(string));
	} catch (...) {
		logF(" Unhandled exception for ByteTag : %s", string.c_str());
	}
}
ListTag::ListTag() {
	static uintptr_t** ListTagVtable = 0x0;
	if (ListTagVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 48 89 08 33 C9 48 89 48 08 48 89 48 10 48 89 48 18 88 48 20 48 89 03");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		ListTagVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
		if (ListTagVtable == 0x0 || sigOffset == 0x0)
			logF("ListTagVtable signature not working!!!");
	}
	memset(this, 0, sizeof(ListTag));  // Avoid overwriting vtable
	uintptr_t* _this = reinterpret_cast<uintptr_t*>(this);
	*_this = (uintptr_t)ListTagVtable;
}
void ListTag::add(std::unique_ptr<Tag> tag) {
	using ListTag__add_t = void(__fastcall*)(ListTag*, std::unique_ptr<Tag>);
	static ListTag__add_t func = reinterpret_cast<ListTag__add_t>(FindSignature("48 89 5C 24 ? 57 48 83 EC 30 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B FA 48 8B D9 48 89 54 24 ? 48 8B 0A 48 8B 01"));
	func(this, std::move(tag));
}
void CompoundTag::write(std::stringstream& builder) {
	builder << MojangsonToken::COMPOUND_START.getSymbol();
	bool start = true;

	class CompoundTagVariant {
	private:
		char pad_0x0[0x20];  //0x0000

	public:
		TextHolder keyName;  //0x0020
		Tag tag;             //0x0040
	};

	__int64* v10 = *(__int64**)(uintptr_t(this) + 0x8);
	__int64* v11 = (__int64*)*v10;
	__int64 i = 0;
	if ((__int64*)*v10 != v10) {
		do {
			if (start) {
				start = false;
			} else {
				builder << MojangsonToken::ELEMENT_SEPERATOR.getSymbol();
			}
			CompoundTagVariant* variant = reinterpret_cast<CompoundTagVariant*>(v11);

			if (variant) {
				builder << variant->keyName.getText() << MojangsonToken::ELEMENT_PAIR_SEPERATOR.getSymbol();
				Handler::handleWrite(&variant->tag, builder);
			}


			__int64** v17 = (__int64**)v11[2];
			if (*((BYTE*)v17 + 0x19)) {
				for (i = v11[1]; !*(BYTE*)(i + 0x19); i = *(__int64*)(i + 8)) {
					if (v11 != *(__int64**)(i + 0x10))
						break;
					v11 = (__int64*)i;
				}
				v11 = (__int64*)i;
			} else {
				v11 = (__int64*)v11[2];
				for (__int64* j = *v17; !*((BYTE*)j + 25); j = (__int64*)*j)
					v11 = j;
			}

		} while (v11 != *(__int64**)(uintptr_t(this) + 0x8));
	}

	/*for (std::map<std::string, Tag>::iterator it = this->map.begin(); it != this->map.end(); it++) {
		if (start) {
			start = false;
		} else {
			builder << MojangsonToken::ELEMENT_SEPERATOR.getSymbol();
		}

		builder << it->first << MojangsonToken::ELEMENT_PAIR_SEPERATOR.getSymbol();
		yeet::handleWrite(&it->second, builder);
	}*/
	builder << MojangsonToken::COMPOUND_END.getSymbol();
}
CompoundTag::CompoundTag() {
	memset(this, 0, sizeof(CompoundTag));
	using constructor_t = void(__fastcall*)(CompoundTag*);
	static constructor_t func = reinterpret_cast<constructor_t>(FindSignature("48 89 5C 24 ? 48 89 4C 24 ? 57 48 83 EC ? 48 8B F9 48 8D 05 ? ? ? ? 48 89 01 48 8D 59 ? 48 89 5C 24 ? 33 C0 48 89 03 48 89 43"));
	func(this);
}
void CompoundTag::put(TextHolder& tag, std::unique_ptr<Tag> value) {
	using CompoundTag__putF = void(__fastcall*)(CompoundTag*, TextHolder&, std::unique_ptr<Tag>);
	static CompoundTag__putF func = reinterpret_cast<CompoundTag__putF>(FindSignature("40 53 56 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 49 8B F8 48 8B DA 48 89 54 24 ? 4C 89 44 24 ? 4D 8B"));
	func(this, tag, std::move(value));
}
void Handler::handleWrite(Tag* value, std::stringstream& builder) {
	switch (static_cast<TagType>(value->getId())) {
	case TagType::BYTE:
		reinterpret_cast<ByteTag*>(value)->write(builder);
		break;
	case TagType::COMPOUND:
		reinterpret_cast<CompoundTag*>(value)->write(builder);
		break;
	case TagType::DOUBLE:
		reinterpret_cast<DoubleTag*>(value)->write(builder);
		break;
	case TagType::FLOAT:
		reinterpret_cast<FloatTag*>(value)->write(builder);
		break;
	case TagType::INT:
		reinterpret_cast<IntTag*>(value)->write(builder);
		break;
	case TagType::LIST:
		reinterpret_cast<ListTag*>(value)->write(builder);
		break;
	case TagType::LONG:
		reinterpret_cast<Int64Tag*>(value)->write(builder);
		break;
	case TagType::SHORT:
		reinterpret_cast<ShortTag*>(value)->write(builder);
		break;
	case TagType::STRING:
		reinterpret_cast<StringTag*>(value)->write(builder);
		break;
	case TagType::BYTE_ARRAY:
		logF("BYTE ARRAY CANNOT HANDLE");
		break;
	case TagType::INT_ARRAY:
		logF("INT ARRAY CANNOT HANDLE");
		break;
	default:
		break;
	}
}