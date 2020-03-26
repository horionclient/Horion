#pragma once
#include "../Utils/Utils.h"
#include "MojangsonToken.h"
#include "TextHolder.h"

class Tag {
public:
	virtual ~Tag() {}

	virtual void deleteChildren() {}

	virtual void write(__int64&) {}

	virtual void load(__int64&) {}

	virtual std::string toString() {
		return this->toString();
	}

	virtual char getId() {
		return this->getId();
	}

	virtual bool equals(Tag& tag) {
		return this->equals(tag);
	}

	virtual void print(std::string, __int64&) {}

	virtual void print(__int64&) {}

	virtual std::unique_ptr<Tag> copy(std::unique_ptr<Tag> tag) {
		return this->copy(std::move(tag));
	}

	virtual __int64 hash() {
		return this->hash();
	}

	/**
	* Writes the value to a StringBuilder buffer.
	* @param builder The buffer to write to
	*/
	void write(std::stringstream& builder);

	/**
	 * Parses and updates the current value to the given string representation
	 * @param string The string representation of the value
	 * @throws MojangsonParseException if the given value cannot be parsed
	 */
	void read(std::string& string);  // throw MojangsonParseException;
};

class Mojangson {
public:
	static inline std::unique_ptr<Tag> parseTag(std::string& mojangson);
};

class Handler {
public:
	static inline void handleWrite(Tag* value, std::stringstream& builder);
};

class Int64Tag : public Tag {
private:
	__int64 value;

public:
	Int64Tag(__int64 value) {
		static uintptr_t** Int64TagVtable = 0x0;
		if (Int64TagVtable == 0x0) {
			uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 49 89 00 48 8B 41 ?? 49 89 40 ?? 48 8B 02 C6 40 ? ? C3");
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

	__int64 getValue() {
		return value;
	}

	void setValue(__int64 value) {
		this->value = value;
	}

	//@Override
	//public Class getValueClass() {
	//    return long.class;
	//}

	void write(std::stringstream& builder) {
		builder << value << MojangsonToken::LONG_SUFFIX.getSymbol();
	}

	void read(std::string& string)  //throws MojangsonParseException
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
};

class IntTag : public Tag {
private:
	int value;

public:
	IntTag(int value) {
		static uintptr_t** IntTagVtable = 0x0;
		if (IntTagVtable == 0x0) {
			uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 49 89 00 8B 41 ?? 41 89 40 ?? 48 8B 02 C6 40 ?? ?? C3");
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

	int getValue() {
		return value;
	}

	void setValue(int value) {
		this->value = value;
	}

	//  public Class getValueClass() {
	//  return int.class;
	//}

	void write(std::stringstream& builder) {
		builder << value;
	}

	void read(std::string& string)  //throws MojangsonParseException
	{
		try {
			value = static_cast<int>(std::stoi(string));
		} catch (...) {
			logF(" Unhandled exception for IntTag : %s", string.c_str());
		}
	}
};

class StringTag : public Tag {
private:
	TextHolder value;

public:
	StringTag(std::string value) {
		static uintptr_t** StringTagVtable = 0x0;
		if (StringTagVtable == 0x0) {
			uintptr_t sigOffset = FindSignature("4C 8D 05 ? ? ? ? 4C 89 00 45 33 C0 4C 89 40 ?? 4C 89 40 ?? 0F 10 41 ?? 0F 11 40 ?? 0F 10 49 ?? 0F 11 48");
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

	std::string getValue() {
		return value.getText();
	}

	void setValue(std::string value) {
		this->value.setText(value);
	}

	void write(std::stringstream& builder) {
		builder << MojangsonToken::STRING_QUOTES.getSymbol() << value.getText() << MojangsonToken::STRING_QUOTES.getSymbol();
	}

	// public Class getValueClass() {
	//  return String.class;
	//  }

	void read(std::string& string)  //throws MojangsonParseException
	{
		char lastChar = string.at(string.length() - 1);
		char firstChar = string.at(0);

		if (firstChar == MojangsonToken::STRING_QUOTES.getSymbol() && lastChar == MojangsonToken::STRING_QUOTES.getSymbol()) {
			value.setText(string.substr(1, string.length() - 2));
		} else {
			value.setText(string);
		}
	}
};

class ShortTag : public Tag {
private:
	short value;

public:
	ShortTag(short value) {
		static uintptr_t** ShortTagVtable = 0x0;
		if (ShortTagVtable == 0x0) {
			uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 49 89 00 0F B7 41 ?? 66 41 89 40 ? 48 8B 02 C6 40 ? ? C3");
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

	short getValue() {
		return value;
	}

	void setValue(short value) {
		this->value = value;
	}

	// public Class getValueClass() {
	//   return short.class;
	//}

	void write(std::stringstream& builder) {
		builder << value << MojangsonToken::SHORT_SUFFIX.getSymbol();
	}

	void read(std::string& string)  //throws MojangsonParseException
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
};

class FloatTag : public Tag {
private:
	float value;

public:
	FloatTag(float value) {
		static uintptr_t** FloatTagVtable = 0x0;
		if (FloatTagVtable == 0x0) {
			uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 07 F3 0F 11 77 ? C6 47 ? ? 48 8B CE E8 ? ? ? ? 48 8D 47 ? 48 8B 4C 24 ? 48 33 CC");
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

	float getValue() {
		return value;
	}

	void setValue(float value) {
		this->value = value;
	}

	//typeid getValueClass() {
	//	return typeid(value);
	//}

	void write(std::stringstream& builder) {
		builder << value << MojangsonToken::FLOAT_SUFFIX.getSymbol();
	}

	void read(std::string& string)  // MojangsonParseException
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
};

class DoubleTag : public Tag {
private:
	double value;

public:
	DoubleTag(double value) {
		static uintptr_t** DoubleTagVtable = 0x0;
		if (DoubleTagVtable == 0x0) {
			uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 49 89 00 48 8B 41 08 49 89 40 08 48 8B 02 C6 40 28 06 C3");
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

	double getValue() {
		return value;
	}

	void setValue(double value) {
		this->value = value;
	}

	//public Class getValueClass() {
	//   return double.class;
	//}

	void write(std::stringstream& builder) {
		builder << value << MojangsonToken::DOUBLE_SUFFIX.getSymbol();
	}

	void read(std::string& string)  //throws MojangsonParseException
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
};

class ByteTag : public Tag {
private:
	char value;

public:
	ByteTag(char value) {
		static uintptr_t** ByteTagVtable = 0x0;
		if (ByteTagVtable == 0x0) {
			uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 48 89 08 C6 40 08 ?? 48 89 03 48 8B C3 48 83 C4 ?? 5B C3 B9 ? ? ? ? E8 ? ? ? ? 48 89 44 24 ?");
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

	char getValue() {
		return value;
	}

	void setValue(char value) {
		this->value = value;
	}

	// public Class getValueClass() {
	//    return byte.class;
	//}

	void write(std::stringstream& builder) {
		builder << (int)value << MojangsonToken::BYTE_SUFFIX.getSymbol();
	}

	void read(std::string& string)  //throws MojangsonParseException
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
};

class ListTag : public Tag {
private:
	uintptr_t firstTag;  //0x0008
	uintptr_t lastTag;   //0x0010
	uintptr_t endAddr;   //0x0018
	size_t id;           //0x20
public:
	inline size_t getListSize() {
		return (lastTag - firstTag) / sizeof(uintptr_t);
	}

	inline Tag* get(std::size_t idx) {
		if (idx >= getListSize())
			return 0;
		return *reinterpret_cast<Tag**>(firstTag + (idx * sizeof(uintptr_t)));
	}

	Tag* operator[](std::size_t idx) {
		return get(idx);
	}

public:
	ListTag() {
		static uintptr_t** ListTagVtable = 0x0;
		if (ListTagVtable == 0x0) {
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 45 00 4D 89 65 ?? 4D 89 65 ?? 4D 89 65 ?? 45 88 65 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 44 89 7C 24");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			ListTagVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (ListTagVtable == 0x0 || sigOffset == 0x0)
				logF("ListTagVtable signature not working!!!");
		}
		memset(this, 0, sizeof(ListTag));  // Avoid overwriting vtable
		uintptr_t* _this = reinterpret_cast<uintptr_t*>(this);
		*_this = (uintptr_t)ListTagVtable;
	}

	void add(std::unique_ptr<Tag> tag) {
		using ListTag__add_t = void(__fastcall*)(ListTag*, std::unique_ptr<Tag>);
		ListTag__add_t func = reinterpret_cast<ListTag__add_t>(FindSignature("40 57 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B FA 48 8B D9 48 89 54 24 ?? 48 8B 0A 48 8B 01 FF 50"));
		func(this, std::move(tag));
	}

	void write(std::stringstream& builder) {
		builder << MojangsonToken::ARRAY_START.getSymbol();
		bool start = true;

		for (size_t i = 0; i < this->getListSize(); i++) {
			if (start) {
				start = false;
			} else {
				builder << MojangsonToken::ELEMENT_SEPERATOR.getSymbol();
			}
			Tag* value = this->get(i);
			Handler::handleWrite(value, builder);
		}
		builder << MojangsonToken::ARRAY_END.getSymbol();
	}

	void read(std::string& string) {
		static constexpr int C_ARRAY_START = 0;    // Parsing context
		static constexpr int C_ARRAY_ELEMENT = 1;  // Parsing context
		int context = C_ARRAY_START;
		std::string tmpval = "";
		int scope = 0;
		bool inString = false;

		for (size_t index = 0; index < string.size(); index++) {
			char character = string.at(index);

			if (character == MojangsonToken::STRING_QUOTES.getSymbol()) {
				inString = !inString;
			}
			if (character == MojangsonToken::WHITE_SPACE.getSymbol()) {
				if (!inString)
					continue;
			}
			if ((character == MojangsonToken::COMPOUND_START.getSymbol() || character == MojangsonToken::ARRAY_START.getSymbol()) && !inString) {
				scope++;
			}
			if ((character == MojangsonToken::COMPOUND_END.getSymbol() || character == MojangsonToken::ARRAY_END.getSymbol()) && !inString) {
				scope--;
			}
			if (context == C_ARRAY_START) {
				if (character != MojangsonToken::ARRAY_START.getSymbol()) {
					//parseException(index, character);
					return;
				}
				context++;
				continue;
			}
			if (context == C_ARRAY_ELEMENT) {
				if ((character == MojangsonToken::ELEMENT_SEPERATOR.getSymbol() || character == MojangsonToken::ARRAY_END.getSymbol()) && scope <= 1 && !inString) {
					if (tmpval.length() == 0) {
						continue;
					}
					std::unique_ptr<Tag> val = std::move(Mojangson::parseTag(tmpval));

					this->add(std::move(val));
					tmpval = "";
					continue;
				}
				tmpval += character;
			}
		}
	}

	// private void parseException(int index, char symbol) throws MojangsonParseException {
	//  throw new MojangsonParseException("Index: " + index + ", symbol: \'" + symbol + "\'", MojangsonParseException.ParseExceptionReason.UNEXPECTED_SYMBOL);
	// }*/
};

class CompoundTag : public Tag {
private:
	//std::map<std::string, Tag> map;//Don't use or Crash
	__int64* map;    //0x0008
	size_t entries;  //0x0010

public:
	CompoundTag() {
		memset(this, 0, sizeof(CompoundTag));
		using constructor_t = void(__fastcall*)(CompoundTag*);
		constructor_t func = reinterpret_cast<constructor_t>(FindSignature("48 89 4C 24 ?? 57 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 48 8D 59"));
		func(this);
	}

	void put(TextHolder& tag, std::unique_ptr<Tag> value) {
		using put_t = void(__fastcall*)(CompoundTag*, TextHolder&, std::unique_ptr<Tag>);
		static put_t func = reinterpret_cast<put_t>(FindSignature("4C 8B DC 53 56 57 48 ?? ?? ?? ?? ?? ?? 49 ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 49 8B F8 48 8B DA 49 89 53 ?? 4D 89 43 ?? 49 8B 30 48 85 F6"));
		func(this, tag, std::move(value));
	}

	/*void putCompound(TextHolder& text, std::unique_ptr<CompoundTag> tag) {
		using putCompound_t = void(__fastcall*)(CompoundTag*, TextHolder&, std::unique_ptr<CompoundTag>);
		static putCompound_t func = reinterpret_cast<putCompound_t>(FindSignature("40 53 56 57 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 49 8B D8 48 8B FA 48 89 54 24 ?? 48 89 5C ?? 40 49 83 38 ?? 74 4A"));
		func(this, text, std::move(tag));
	}

	std::unique_ptr<CompoundTag> clone(CompoundTag** tag) {
		using clone_t = std::unique_ptr<CompoundTag>(__fastcall*)(CompoundTag*, CompoundTag**);
		static clone_t func = reinterpret_cast<clone_t>(FindSignature("48 89 54 24 ?? 57 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 8B FA 48 8B D9 C7 44 ?? ?? ?? ?? ?? ?? 33 C0 48 89 02 48 8B CA"));
		return func(this, std::move(tag));
	}*/

	/*CompoundTag(std::map<std::string, Tag>& map) {
		this->map = map;
	}*/

	void write(std::stringstream& builder) {
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

	void read(std::string& string) {
		static constexpr int C_COMPOUND_START = 0;       // Parsing context
		static constexpr int C_COMPOUND_PAIR_KEY = 1;    // Parsing context
		static constexpr int C_COMPOUND_PAIR_VALUE = 2;  // Parsing context

		int context = C_COMPOUND_START;
		std::string tmp_key = "", tmp_val = "";
		int scope = 0;
		bool inString = false;

		for (int index = 0; index < string.length(); index++) {
			char character = string.at(index);

			if (character == MojangsonToken::STRING_QUOTES.getSymbol()) {
				inString = !inString;
			}
			if (character == MojangsonToken::WHITE_SPACE.getSymbol()) {
				if (!inString)
					continue;
			}
			if ((character == MojangsonToken::COMPOUND_START.getSymbol() || character == MojangsonToken::ARRAY_START.getSymbol()) && !inString) {
				scope++;
			}
			if ((character == MojangsonToken::COMPOUND_END.getSymbol() || character == MojangsonToken::ARRAY_END.getSymbol()) && !inString) {
				scope--;
			}
			if (context == C_COMPOUND_START) {
				if (character != MojangsonToken::COMPOUND_START.getSymbol()) {
					//parseException(index, character);
					return;
				}
				context++;
				continue;
			}
			if (context == C_COMPOUND_PAIR_KEY) {
				if (character == MojangsonToken::ELEMENT_PAIR_SEPERATOR.getSymbol() && scope <= 1) {
					context++;
					continue;
				}
				tmp_key += character;
				continue;
			}
			if (context == C_COMPOUND_PAIR_VALUE) {
				if ((character == MojangsonToken::ELEMENT_SEPERATOR.getSymbol() || character == MojangsonToken::COMPOUND_END.getSymbol()) && scope <= 1 && !inString) {
					context = C_COMPOUND_PAIR_KEY;
					this->put(TextHolder(tmp_key), std::move(Mojangson::parseTag(tmp_val)));
					tmp_key = tmp_val = "";
					continue;
				}
				tmp_val += character;
			}
		}
	}

	// private void parseException(int index, char symbol) throws MojangsonParseException {
	//   throw new MojangsonParseException("Index: " + index + ", symbol: \'" + symbol + "\'", MojangsonParseException.ParseExceptionReason.UNEXPECTED_SYMBOL);
	//}
};

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

/**
	 * Detects the Tag type of the Mojangson string, and parses it. Convenience method for other
	 * parse methods.
	 *
	 * <p>This method will fall back to an IntTag if it could not find an appropriate Tag type, and
	 * to String if the value could not be parsed as an Integer either.
	 *
	 * @param mojangson The Mojangson string
	 * @return The parsed NBT Tag
	 * @throws MojangsonParseException if the given Mojangson string could not be parsed.
	 */
std::unique_ptr<Tag> Mojangson::parseTag(std::string& mojangson) {
	if (mojangson.front() == MojangsonToken::COMPOUND_START.getSymbol() && mojangson.back() != MojangsonToken::COMPOUND_END.getSymbol()) {
		mojangson += '}';
	}
	if (mojangson.front() == MojangsonToken::STRING_QUOTES.getSymbol() && mojangson.back() == MojangsonToken::STRING_QUOTES.getSymbol()) {
		std::unique_ptr<StringTag> val = std::make_unique<StringTag>("");
		val->read(mojangson);
		return val;
	}
	if (mojangson.front() == MojangsonToken::ARRAY_START.getSymbol() && mojangson.back() == MojangsonToken::ARRAY_END.getSymbol()) {
		std::unique_ptr<ListTag> val = std::make_unique<ListTag>();
		val->read(mojangson);
		return val;
	}
	if (mojangson.front() == MojangsonToken::COMPOUND_START.getSymbol() && mojangson.back() == MojangsonToken::COMPOUND_END.getSymbol()) {
		std::unique_ptr<CompoundTag> val = std::make_unique<CompoundTag>();
		val->read(mojangson);
		return val;
	}
	if (mojangson.back() == MojangsonToken::BYTE_SUFFIX.getSymbol()) {
		std::unique_ptr<ByteTag> val = std::make_unique<ByteTag>(0);
		val->read(mojangson);
		return val;
	}
	if (mojangson.back() == MojangsonToken::DOUBLE_SUFFIX.getSymbol() /*|| mojangson.contains(".")*/) {
		std::unique_ptr<DoubleTag> val = std::make_unique<DoubleTag>(0);
		val->read(mojangson);
		return val;
	}
	if (mojangson.back() == MojangsonToken::LONG_SUFFIX.getSymbol()) {
		std::unique_ptr<Int64Tag> val = std::make_unique<Int64Tag>(0);
		val->read(mojangson);
		return val;
	}
	if (mojangson.back() == MojangsonToken::FLOAT_SUFFIX.getSymbol()) {
		std::unique_ptr<FloatTag> val = std::make_unique<FloatTag>(0.f);
		val->read(mojangson);
		return val;
	}
	if (mojangson.back() == MojangsonToken::SHORT_SUFFIX.getSymbol()) {
		std::unique_ptr<ShortTag> val = std::make_unique<ShortTag>(0);
		val->read(mojangson);
		return val;
	}
	std::unique_ptr<IntTag> val = std::make_unique<IntTag>(0);
	val->read(mojangson);
	return val;
}
