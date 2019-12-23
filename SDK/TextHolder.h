#pragma once

#include <string>

class TextHolder
{	
public:

	union {
		char inlineText[16]; //0x0000 
		char *pText; //0x0000 
	};

	size_t textLength; //0x0010 
	size_t alignedTextLength; //0x0018

	TextHolder() {
		memset(this, 0, sizeof(TextHolder));
	}

	TextHolder(TextHolder const& copy) {
		memset(this, 0, sizeof(TextHolder));
		textLength = copy.textLength;
		alignedTextLength = copy.alignedTextLength;
		if(copy.textLength < 16)
			strcpy_s(inlineText, 16, copy.inlineText);
		else {
			size_t size = strlen(copy.pText) + 1;
			pText = reinterpret_cast<char*>(malloc(size));
			if (pText != 0x0 && copy.pText != 0x0) {
				strcpy_s(pText, size, copy.pText);
			}
		}
	}

	TextHolder& operator=(TextHolder const& copy) {
		if (textLength >= 16 && pText != nullptr) {
			free(pText);
		}
		memset(this, 0, sizeof(TextHolder));
		textLength = copy.textLength;
		alignedTextLength = copy.alignedTextLength;
		if (copy.textLength < 16)
			strcpy_s(inlineText, 16, copy.inlineText);
		else {
			size_t size = strlen(copy.pText) + 1;
			pText = reinterpret_cast<char*>(malloc(size));
			if (pText != 0x0 && copy.pText != 0x0) {
				strcpy_s(pText, size, copy.pText);
			}
		}
		return *this;
	}

	TextHolder(std::string str) {
		memset(this, 0, sizeof(TextHolder));
		textLength = str.size();
		alignedTextLength = textLength | 0xF;
		if (str.size() < 16)
			strcpy_s(inlineText, 16, str.c_str());
		else {
			size_t size = str.size();
			pText = reinterpret_cast<char*>(malloc(size + 1));
			if (pText != 0x0) {
				strcpy_s(pText, size + 1, str.c_str());
			}
		}
	}

	TextHolder(void* ptr, size_t sizeOfData) {
		memset(this, 0, sizeof(TextHolder));
		textLength = sizeOfData;
		alignedTextLength = sizeOfData;
		if (textLength < 16)
			memcpy(inlineText, ptr, sizeOfData);
		else
			pText = reinterpret_cast<char*>(ptr);
	}

	~TextHolder() {
		if (textLength >= 16 && pText != nullptr) {
			free(pText);
		}
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
		memset(this, 0, sizeof(TextHolder));
		textLength = str.size();
		alignedTextLength = textLength | 0xF;
		if (str.size() < 16)
			strcpy_s(inlineText, 16, str.c_str());
		else {
			size_t size = str.size();
			char* ptr = reinterpret_cast<char*>(malloc(size + 1));
			if (ptr != 0x0) {
				strcpy_s(ptr, size + 1, str.c_str());
			}

			pText = ptr;
		}
	}

	void resetWithoutDelete() {
		memset(this, 0, sizeof(TextHolder));
	}
};