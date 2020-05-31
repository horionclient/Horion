#pragma once
#include "MojangsonToken.h"
#include "TextHolder.h"
#include <sstream>
#include <memory>

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
	static void handleWrite(Tag* value, std::stringstream& builder);
};

class Int64Tag : public Tag {
private:
	__int64 value;

public:
	Int64Tag(__int64 value);

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

	void read(std::string& string);
};

class IntTag : public Tag {
private:
	int value;

public:
	IntTag(int value);

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

	void read(std::string& string);
};

class StringTag : public Tag {
private:
	TextHolder value;

public:
	StringTag(std::string value);

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
	ShortTag(short value);

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

	void read(std::string& string);
};

class FloatTag : public Tag {
private:
	float value;

public:
	FloatTag(float value);

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

	void read(std::string& string);
};

class DoubleTag : public Tag {
private:
	double value;

public:
	DoubleTag(double value);

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

	void read(std::string& string);
};

class ByteTag : public Tag {
private:
	char value;

public:
	ByteTag(char value);

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

	void read(std::string& string);
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
	ListTag();

	void add(std::unique_ptr<Tag> tag);

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
	CompoundTag();

	void put(TextHolder& tag, std::unique_ptr<Tag> value);

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

	void write(std::stringstream& builder);

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
					TextHolder t(tmp_key);
					this->put(t, std::move(Mojangson::parseTag(tmp_val)));
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
