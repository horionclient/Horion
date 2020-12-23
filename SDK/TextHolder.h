#pragma once

#include <string>

class TextHolder {
public:
	union {
		char inlineText[16];  //0x0000
		char* pText;          //0x0000
	};

	size_t textLength;         //0x0010
	size_t alignedTextLength;  //0x0018

	TextHolder() {
		memset(this, 0, sizeof(TextHolder));
	}

	TextHolder(TextHolder const& copy) {
		memset(this, 0, sizeof(TextHolder));
		textLength = copy.textLength;
		alignedTextLength = copy.alignedTextLength;
		if (copy.textLength < 16)
			memcpy(inlineText, copy.inlineText, 16);
		else {
			size_t size = textLength + 1;

			if (size + 1 >= 0x1000)
				size += 8;

			pText = reinterpret_cast<char*>(malloc(size + 1));
			alignedTextLength = size;
			if (size + 1 >= 0x1000) {
				*reinterpret_cast<char**>(pText) = pText;
				pText += 8;
			}

			if (pText != 0x0 && copy.pText != 0x0) {
				memcpy(pText, copy.pText, size);
				pText[size] = 0;
			}
		}
	}

	TextHolder& operator=(TextHolder const& copy) {
		deleteText();
		memset(this, 0, sizeof(TextHolder));
		textLength = copy.textLength;
		alignedTextLength = copy.alignedTextLength;
		if (copy.textLength < 16)
			memcpy(inlineText, copy.inlineText, 16);
		else {
			size_t size = textLength + 1;

			if (size + 1 >= 0x1000)
				size += 8;

			pText = reinterpret_cast<char*>(malloc(size + 1));
			alignedTextLength = size;
			if (size + 1 >= 0x1000) {
				*reinterpret_cast<char**>(pText) = pText;
				pText += 8;
			}

			if (pText != 0x0 && copy.pText != 0x0) {
				memcpy(pText, copy.pText, textLength);
				pText[textLength] = 0;
			}
		}
		return *this;
	}

	TextHolder(std::string str) {
		memset(this, 0, sizeof(TextHolder));
		textLength = str.size();
		alignedTextLength = textLength | 0xF;
		if (str.size() < 16) {
			memcpy(inlineText, str.c_str(), str.size());
			if (str.size() < 15)
				inlineText[str.size()] = 0;
		} else {
			size_t size = str.size();
			if (size + 1 >= 0x1000)
				size += 8;

			pText = reinterpret_cast<char*>(malloc(size + 1));
			alignedTextLength = size;
			if (size + 1 >= 0x1000) {
				*reinterpret_cast<char**>(pText) = pText;
				pText += 8;
			}

			if (pText != 0x0) {
				memcpy(pText, str.c_str(), str.size());
				pText[str.size()] = 0;
			}
		}
	}

	TextHolder(size_t allocSize) {
		memset(this, 0, sizeof(TextHolder));
		textLength = 0;
		alignedTextLength = allocSize | 0xF;
		if (alignedTextLength < 16) {
			memset(&inlineText, 0, 16);
		} else {
			size_t size = alignedTextLength;
			if (size + 1 >= 0x1000)
				size += 8;

			pText = reinterpret_cast<char*>(malloc(size + 1));
			alignedTextLength = size;
			if (size + 1 >= 0x1000) {
				*reinterpret_cast<char**>(pText) = pText;
				
				pText += 8;
				memset(pText, 0, alignedTextLength - 8);
			}else
				memset(pText, 0, alignedTextLength);
		}
	}

	TextHolder(void* ptr, size_t sizeOfData) {
		memset(this, 0, sizeof(TextHolder));
		textLength = sizeOfData;
		alignedTextLength = sizeOfData;
		if (alignedTextLength < 16)
			memcpy(inlineText, ptr, sizeOfData);
		else
			pText = reinterpret_cast<char*>(ptr);
	}

	void deleteText() {
		if (alignedTextLength >= 16 && pText != nullptr) {
			if (alignedTextLength + 1 >= 0x1000) {
				pText = *reinterpret_cast<char**>(reinterpret_cast<__int64>(pText) - 8);
			}
			free(pText);
		}
	}

	~TextHolder() {
		this->deleteText();
	}

	char* getText() {
		if (alignedTextLength < 16)
			return this->inlineText;
		else
			return this->pText;
	}

	size_t getTextLength() {
		return textLength;
	}

	void setText(std::string str) {
		this->deleteText();
		memset(this, 0, sizeof(TextHolder));
		textLength = str.size();
		alignedTextLength = textLength | 0xF;
		if (str.size() < 16) {
			memcpy(inlineText, str.c_str(), str.size());
			if (str.size() < 15)
				inlineText[str.size()] = 0;
		} else {
			size_t size = str.size();
			if (size + 1 >= 0x1000)
				size += 8;

			pText = reinterpret_cast<char*>(malloc(size + 1));
			alignedTextLength = size;
			if (size + 1 >= 0x1000) {
				*reinterpret_cast<char**>(pText) = pText;
				pText += 8;
			}

			if (pText != 0x0) {
				memcpy(pText, str.c_str(), str.size());
				pText[str.size()] = 0;
			}
		}
	}

	void resetWithoutDelete() {
		memset(this, 0, sizeof(TextHolder));
	}
};

class GamerTextHolder {
public:
	union {
		char inlineText[16];  //0x0000
		char* pText;          //0x0000
	};

	size_t textLength;         //0x0010
	size_t alignedTextLength;  //0x0018

	GamerTextHolder() {
		memset(this, 0, sizeof(GamerTextHolder));
	}

	char* getText() {
		if (alignedTextLength < 16)
			return this->inlineText;
		else
			return this->pText;
	}

	void copyFrom(TextHolder* copy);

	void deleteText();
};